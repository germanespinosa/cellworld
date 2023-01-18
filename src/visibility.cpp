#include <cell_world/visibility.h>
#include <math.h>
#include <cell_world/geometry.h>

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

    Cell_group Coordinates_visibility_cone::visible_cells(const Cell &src, float theta) {
        Cell_group res;
        for (auto &dst: visibility[src]) {
            if (is_visible(src, theta, dst)) {
                res.add(dst);
            }
        }
        return res;
    }

    bool Coordinates_visibility_cone::is_visible(const Cell &src, float theta, const Cell &dst) const {
        if (!visibility[src].contains(dst)) return false;
        auto angle = src.location.atan(dst.location);
        auto theta_dif = angle_difference(angle, theta);
        return theta_dif <= visual_angle / 2;
    }

    Coordinates_visibility_cone::Coordinates_visibility_cone(const Graph &visibility, float visual_angle) :
            visibility(visibility),
            visual_angle(visual_angle) {

    }

    Location_visibility::Location_visibility(const Cell_group &cells, const Shape &cell_shape,
                                             const Transformation &cell_transformation):
            Location_visibility(cell_shape, cell_transformation){
        update_occlusions(cells);
    }

    bool cell_world::Location_visibility::is_visible(const Location &src, const Location &dst) const {
        float theta = src.atan(dst);
        float dist = src.dist(dst);
        for (auto &o: occlusions) {
            if (o.is_between(src, theta, dist)){
                return false;
            }
        }
        return true;
    }

    bool cell_world::Location_visibility::is_visible(const Location &src, float src_theta, float src_cone, const Location &dst) const {
        float theta = src.atan(dst);
        if (angle_difference(src_theta, theta) > src_cone / 2) return false;
        float dist = src.dist(dst);
        for (auto &o: occlusions) {
            if (o.is_between(src, theta, dist)){
                return false;
            }
        }
        return true;
    }

    Location_visibility::Location_visibility(const Shape &cell_shape, const Transformation &cell_transformation):
    cell_shape(cell_shape),
    cell_transformation(cell_transformation){

    }

    void Location_visibility::update_occlusions(const Cell_group &cells) {
        auto occluded_cells = cells.occluded_cells();
        occlusions.clear();
        cell_locations.clear();
        for (const Cell &cell: occluded_cells) {
            occlusions.emplace_back(cell.location,
                                    cell_shape.sides,
                                    cell_transformation.size / 2,
                                    to_radians(cell_transformation.rotation));
            cell_locations.push_back(cell.location);
        }

    }

    Json_bool_vector Location_visibility::is_visible_multi(const Location &src, float src_theta, float src_cone, const Location_list &dst_multi) const {
        Json_bool_vector result(dst_multi.size(), false);
        for (unsigned int i = 0; i < result.size(); i++){
            result[i] = is_visible(src, src_theta, src_cone, dst_multi[i]);
        }
        return result;
    }

    Location_list Location_visibility::get_visible_locations(const Location &src, float src_theta, float src_cone) const {
        return get_visible_locations(src, src_theta, src_cone, cell_locations);
    }

    Location_list Location_visibility::get_visible_locations(const Location &src, float src_theta, float src_cone, const Location_list &dst_multi) const {
        Location_list result;
        for (auto &dst:dst_multi){
            if (is_visible(src, src_theta, src_cone, dst))
            {
                result.push_back(dst);
            }
        }
        return result;
    }

    Location_list Location_visibility::get_visible_locations(const Location &src) const {
        return get_visible_locations(src, cell_locations);
    }

    Location_list Location_visibility::get_visible_locations(const Location &src, const Location_list &dst_multi) const {
        Location_list result;
        for (auto &dst:dst_multi){
            if (is_visible(src, dst))
            {
                result.push_back(dst);
            }
        }
        return result;
    }

    Json_unsigned_int_vector
    Location_visibility::is_visible_multi_index(const Location &src, float src_theta, float src_cone, const Location_list &dst_multi) const {
        Json_unsigned_int_vector result;
        for (unsigned int i = 0; i < dst_multi.size(); i++){
            if(is_visible(src, src_theta, src_cone, dst_multi[i])) result.push_back(i);
        }
        return result;
    }
}

