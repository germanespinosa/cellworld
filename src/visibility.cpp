#include <cell_world/visibility.h>
#include <math.h>

using namespace std;

namespace cell_world {

    Graph Visibility::create_graph(const Cell_group &cell_group, const Cell_descriptor &descriptor) {
        if (cell_group.empty()) return {};
        Cell_group occlusions = cell_group.occluded_cells();
        Cell_group free_cells = cell_group.free_cells(); // filters occluded
        Graph vi(cell_group);
        for (const Cell &source:free_cells) { //only not occluded
            vi[source].add(source); //cell is visible to itself
            for (const Cell &destination:free_cells) { //only not occluded
                if( destination.id < source.id) continue;
                bool blocked = false;
                if (source!=destination) {
                    double dist = source.location.dist(destination.location); // distance between source & destination
                    for (const Cell &occlusion:occlusions) {
                        if (occlusion.location.dist(source.location, destination.location) <= descriptor.radius &&
                                occlusion.location.dist(source.location) < dist &&
                                occlusion.location.dist(destination.location) < dist) {
                            blocked = true;
                            break;
                        }
                    }
                }
                if (!blocked) {
                    vi[source].add(destination);
                    vi[destination].add(source);
                }
            }
        }
        return vi;
    }

    Graph Visibility::invert(const Graph &vi) {
        Graph iv(vi.cells);
        Cell_group free_cells = iv.cells.free_cells();
        for (auto &s:free_cells) {
            for (auto &d:free_cells) {
                if (!vi[s].contains(d)) {
                    iv[s].add(d);
                }
            }
        }
        return iv;
    }

    double cell_world::Visibility_cone::angle_difference(double a1, double a2) {
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

    int Visibility_cone::direction(double a1, double a2) {
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

    double cell_world::Visibility_cone::normalize(double angle) {
        while (angle < 0) angle += 2.0 * M_PI;
        while (angle > 2 * M_PI) angle -= 2.0 * M_PI;
        return angle;
    }

    double cell_world::Visibility_cone::to_radians(double degrees) {
        return normalize((degrees-180) / 360.0 * 2.0 * M_PI);
    }

    double cell_world::Visibility_cone::to_degrees(double radians) {
        return normalize_degrees(radians *  360.0 / (2.0 * M_PI) );
    }

    double cell_world::Visibility_cone::normalize_degrees(double angle) {
        while (angle < -180.0) angle += 360.0;
        while (angle > 180.0) angle -= 360.0;
        return angle;
    }

    Cell_group cell_world::Visibility_cone::visible_cells(const Cell &src, double theta) {
        Cell_group res;
        for (auto &dst: visibility[src]) {
            if (is_visible(src,theta, dst)) {
                res.add(dst);
            }
        }
        return res;
    }

    bool cell_world::Visibility_cone::is_visible(const Cell &src, double theta, const Cell &dst) {
        if (!visibility[src].contains(dst)) return false;
        auto angle = src.location.atan(dst.location);
        auto theta_dif = angle_difference(angle, theta);
        return theta_dif <= visual_angle / 2;
    }

    Visibility_cone::Visibility_cone(const Graph &visibility, double visual_angle):
    visibility(visibility),
    visual_angle(visual_angle){

    }
}