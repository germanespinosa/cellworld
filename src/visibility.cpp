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

    Cell_group Coordinates_visibility_cone::visible_cells(const Cell &src, double theta) {
        Cell_group res;
        for (auto &dst: visibility[src]) {
            if (is_visible(src, theta, dst)) {
                res.add(dst);
            }
        }
        return res;
    }

    bool Coordinates_visibility_cone::is_visible(const Cell &src, double theta, const Cell &dst) const {
        if (!visibility[src].contains(dst)) return false;
        auto angle = src.location.atan(dst.location);
        auto theta_dif = angle_difference(angle, theta);
        return theta_dif <= visual_angle / 2;
    }

    Coordinates_visibility_cone::Coordinates_visibility_cone(const Graph &visibility, double visual_angle) :
            visibility(visibility),
            visual_angle(visual_angle) {

    }

    Location_visibility::Location_visibility(const Cell_group &cells, const Shape &cell_shape,
                                             const Transformation &cell_transformation) {
        auto occluded_cells = cells.occluded_cells();
        for (auto &cell: occluded_cells) {
            occlusions.emplace_back(cell.get().location, cell_shape.sides, cell_transformation.size / 2,
                                    to_radians(cell_transformation.rotation));
        }
    }

    bool cell_world::Location_visibility::is_visible(const Location &src, const Location &dst) const {
        double theta = src.atan(dst);
        double dist = src.dist(dst);
        for (auto &o: occlusions) {
            if (o.is_between(src,theta, dist)){
                return false;
            }
        }
        return true;
    }
}

