#include <cell_world/shape.h>

namespace cell_world {
    //SHAPE

    Shape::Shape() = default;

    Shape::Shape(int sides):
            sides(sides){

    }

    //POLYGON

    Polygon::Polygon() = default;

    Polygon::Polygon(const Location &center, unsigned int sides, double radius, double rotation) :
            center(center),
            radius(radius){
        double theta = to_radians(rotation);
        double inc = to_radians(360.0 / double(sides));
        for (unsigned int s = 0; s < sides; s++) {
            auto c = center;
            vertices.emplace_back(c.move(theta, radius));
            theta += inc;
        }
    }

    Polygon::Polygon(const Location &center,const Shape &shape, const Transformation &transformation) :
            Polygon(center, shape.sides, transformation.size / 2, transformation.rotation)
    {

    }


    bool Polygon::contains(const Location &location) const {
        auto dist = center.dist(location);
        if (dist>radius) return false;
        auto inner_radius = center.dist(vertices[0],vertices[1]);
        if (dist<inner_radius) return true;
        for (int i=1; i<vertices.size(); i++)
            if (segments_intersect(center,location,vertices[i],vertices[i-1]))
                return false;
        if (segments_intersect(center,location,vertices[vertices.size()-1],vertices[0]))
            return false;
        return true;
    }


    Polygon Polygon::move(const Location &dst) const {
        Polygon p;
        p = *this;
        auto dif = dst - center;
        return p += dif;
    }

    Polygon Polygon::move(double theta, double dist) const {
        return move(center + Location().move(theta,dist));
    }

    Polygon Polygon::operator+=(const Location &dif) {
        center = center + dif;
        radius = radius;
        for (auto &v: vertices){
            v += dif;
        }
        return *this;
    }

    Polygon &Polygon::operator=(const Polygon &poly) {
        center = poly.center;
        radius = poly.radius;
        vertices = poly.vertices;
        return *this;
    }

    Polygon::Polygon(const Polygon &p) {
        *this = p;
    }

    bool Polygon::is_between(const Location &src, const Location &dst) const {
        double theta = src.atan(dst);
        double dist = src.dist(dst);
        return is_between(src, theta, dist);
    }

    bool Polygon::is_between(const Location &src, double theta, double dist) const {
        double dist_center = src.dist(center);
        if (dist < dist_center - radius ) return false;
        double theta_center = src.atan(center);
        auto diff_theta_center = angle_difference(theta,theta_center);
        auto direction_center = direction(theta, theta_center);
        for (auto &v: vertices) {
            double vertex_distance = src.dist(v);
            if (vertex_distance < dist) {
                double theta_vertex = src.atan(v);
                auto direction_vertex = direction(theta, theta_vertex);
                if (direction_center == -direction_vertex) {
                    auto diff_theta_vertex = angle_difference(theta,theta_vertex);
                    if (diff_theta_center + diff_theta_vertex < M_PI)
                        return true;
                }
            }
        }
        return false;
    }

    bool Polygon::contains(const Polygon &polygon) const {
        for(auto &v:polygon.vertices)
            if (!contains(v)) return false;
        return true;
    }

    bool Polygon::overlaps(const Polygon &polygon) const {
        for(auto &v:polygon.vertices)
            if (contains(v)) return true;
        return false;
    }

    //POLYGON LIST

    Polygon_list::Polygon_list(const Location_list &centers, const Shape &shape, const Transformation &transformation) {
        for (auto &center:centers ) emplace_back(center, shape, transformation);
    }

    bool Polygon_list::contains(const Location &location) const {
        for (auto &p : *this) if (p.contains(location)) return true;
        return false;
    }

    //SPACE

    Space::Space() = default;

    Space::Space(const Location &center, const Shape &shape, const Transformation &transformation) :
            center(center),
            shape(shape),
            transformation(transformation){

    }

    Location Space::transform(const Location &location, const Space &src_space) const {
        auto source_dist_center = src_space.center.dist(location);
        auto source_theta = src_space.center.atan(location);
        auto size_ratio = transformation.size / src_space.transformation.size;
        auto rotation = to_radians(transformation.rotation - src_space.transformation.rotation);
        auto new_location = center;
        return new_location.move(rotation+source_theta, source_dist_center * size_ratio);
    }

}