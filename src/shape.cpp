#include <cell_world/shape.h>
#include <cell_world/geometry.h>

namespace cell_world {
    //SHAPE

    Shape::Shape() = default;

    Shape::Shape(int sides):
            sides(sides){

    }

    //POLYGON

    Polygon::Polygon() = default;

    Polygon::Polygon(const Location &center, unsigned int sides, float radius, float rotation) :
            center(center),
            radius(radius){
        float theta = to_radians(rotation);
        float inc = to_radians(360.0 / float(sides));
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
        for (unsigned int i=1; i<vertices.size(); i++)
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

    Polygon Polygon::move(float theta, float dist) const {
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
        float theta = src.atan(dst);
        float dist = src.dist(dst);
        return is_between(src, theta, dist);
    }

    bool Polygon::is_between(const Location &src, float theta, float dist) const {
        float dist_center = src.dist(center);
        if (dist < dist_center - radius ) return false; // destination closer than the polygon

        float polygon_shade_angle = atan2(dist_center,  radius);
        float theta_center = src.atan(center);
        auto diff_theta_center = angle_difference(theta, theta_center);
        if (diff_theta_center > polygon_shade_angle) return false; // outside the circular shade

        auto direction_center = direction(theta, theta_center);
        for (auto &v: vertices) {
            float vertex_distance = src.dist(v);
            if (vertex_distance < dist) {
                float theta_vertex = src.atan(v);
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

    Location Space::scale(const Location &location, const Scale &scale) {
        return (location - center) * scale + center;
    }

}