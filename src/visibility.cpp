#include <cell_world/visibility.h>
#include <math.h>

using namespace std;

namespace cell_world {

    Graph Coordinates_visibility::create_graph(const Cell_group &cell_group,
                                               const Shape &shape,
                                               const Transformation &transformation) {
        if (cell_group.empty()) return {};
        Cell_group occlusions = cell_group.occluded_cells();
        Location_visibility lv = Location_visibility(occlusions, shape, transformation);
        Cell_group free_cells = cell_group.free_cells(); // filters occluded
        Graph vi(cell_group);
        for (const Cell &source: free_cells) { //only not occluded
            vi[source].add(source); //cell is visible to itself
            for (const Cell &destination: free_cells) { //only not occluded
                if (destination.id < source.id) continue;
                if (source != destination) {
                    if (lv.is_visible(source.location, destination.location)) {
                        vi[source].add(destination);
                        vi[destination].add(source);
                    }
                }
            }
        }
        return vi;
    }

    Graph Coordinates_visibility::invert(const Graph &vi) {
        Graph iv(vi.cells);
        Cell_group free_cells = iv.cells.free_cells();
        for (auto &s: free_cells) {
            for (auto &d: free_cells) {
                if (!vi[s].contains(d)) {
                    iv[s].add(d);
                }
            }
        }
        return iv;
    }

    double Visibility::angle_difference(double a1, double a2) {
        a1 = normalize(a1);
        a2 = normalize(a2);
        if (a1 > a2) {
            auto d = a1 - a2;
            if (d < M_PI) return d;
            else return a2 + M_PI * 2.0 - a1;
        } else {
            auto d = a2 - a1;
            if (d < M_PI) return d;
            else return a1 + M_PI * 2.0 - a2;
        }
    }

    int Visibility::direction(double a1, double a2) {
        a1 = normalize(a1);
        a2 = normalize(a2);
        if (a1 > a2) {
            auto d = a1 - a2;
            if (d < M_PI) return 1;
            else return -1;
        } else {
            auto d = a2 - a1;
            if (d < M_PI) return -1;
            else return 1;
        }
    }

    double Visibility::normalize(double angle) {
        while (angle < 0) angle += 2.0 * M_PI;
        while (angle > 2 * M_PI) angle -= 2.0 * M_PI;
        return angle;
    }

    double Visibility::to_radians(double degrees) {
        return normalize((degrees - 180) / 360.0 * 2.0 * M_PI);
    }

    double Visibility::to_degrees(double radians) {
        return normalize_degrees(radians * 360.0 / (2.0 * M_PI));
    }

    double Visibility::normalize_degrees(double angle) {
        while (angle < -180.0) angle += 360.0;
        while (angle > 180.0) angle -= 360.0;
        return angle;
    }

    bool Visibility::is_occluding(const Location &src, const Location &dst, const Polygon &occlusion) {
        double theta = src.atan(dst);
        double dist = src.dist(dst);
        return is_occluding(src, theta, dist, occlusion);
    }

    bool Visibility::is_occluding(const Location &src, double theta, double dist, const Polygon &occlusion) {
        double dist_center = src.dist(occlusion.center);
        if (dist < dist_center - occlusion.radius ) return false;
        double theta_center = src.atan(occlusion.center);
        auto diff_theta_center = Visibility::angle_difference(theta,theta_center);
        auto direction_center = Visibility::direction(theta, theta_center);
        for (auto &v: occlusion.vertices) {
            double vertex_distance = src.dist(v);
            if (vertex_distance < dist) {
                double theta_vertex = src.atan(v);
                auto direction_vertex = Visibility::direction(theta, theta_vertex);
                if (direction_center == -direction_vertex) {
                    auto diff_theta_vertex = Visibility::angle_difference(theta,theta_vertex);
                    if (diff_theta_center + diff_theta_vertex < M_PI)
                        return true;
                }
            }
        }
        return false;
    }

    Cell_group Coordinates_visibility_cone::visible_cells(const Cell &src, double theta) {
        Cell_group res;
        for (auto &dst: visibility[src]) {
            if (is_visible(src, theta, dst)) {
                res.add(dst);
            }
        }
        return res;
    }

    bool Coordinates_visibility_cone::is_visible(const Cell &src, double theta, const Cell &dst) {
        if (!visibility[src].contains(dst)) return false;
        auto angle = src.location.atan(dst.location);
        auto theta_dif = Visibility::angle_difference(angle, theta);
        return theta_dif <= visual_angle / 2;
    }

    Coordinates_visibility_cone::Coordinates_visibility_cone(const Graph &visibility, double visual_angle) :
            visibility(visibility),
            visual_angle(visual_angle) {

    }

    Location_visibility::Location_visibility(const Cell_group &occluded_cells, const Shape &cell_shape,
                                             const Transformation &cell_transformation) {
        for (auto &cell: occluded_cells) {
            occlusions.emplace_back(cell.get().location, cell_shape.sides, cell_transformation.size / 2,
                                    Visibility::to_radians(cell_transformation.rotation));
        }
    }

    bool cell_world::Location_visibility::is_visible(const Location &src, const Location &dst) {
        double theta = src.atan(dst);
        double dist = src.dist(dst);
        for (auto &o: occlusions) {
            if (Visibility::is_occluding(src,theta, dist,o)){
                return false;
            }
        }
        return true;
    }
}

