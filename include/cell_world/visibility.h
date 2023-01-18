#pragma once
#include <cell_world/types.h>
#include <cell_world/cell_group.h>
#include <cell_world/graph.h>
#include <cell_world/shape.h>

namespace cell_world{

    struct Coordinates_visibility{
        static Graph create_graph(const Cell_group &, const Shape &, const Transformation &);
        static Graph invert(const Graph &);
    };

    struct Coordinates_visibility_cone{
        Coordinates_visibility_cone (const Graph &, float);
        Cell_group visible_cells(const Cell &, float);
        bool is_visible(const Cell &, float, const Cell &) const;
        const Graph &visibility;
        const float visual_angle;
    };

    struct Location_visibility{
        Location_visibility(const Shape &, const Transformation &);
        Location_visibility(const Cell_group &, const Shape &, const Transformation &);
        [[nodiscard]] bool is_visible(const Location &, const Location &) const;
        bool is_visible(const Location &, float, float, const Location &) const;
        Json_bool_vector is_visible_multi(const Location &, float, float, const Location_list &) const;
        Json_unsigned_int_vector is_visible_multi_index(const Location &, float, float, const Location_list &) const;
        void update_occlusions(const Cell_group &);
        Polygon_list occlusions;
        Shape cell_shape;
        Transformation cell_transformation;
        Location_list cell_locations;
        Location_list get_visible_locations(const Location &, float, float) const;
        Location_list get_visible_locations(const Location &, float, float, const Location_list &) const;
        Location_list get_visible_locations(const Location &) const;
        Location_list get_visible_locations(const Location &, const Location_list &) const;
    };
}