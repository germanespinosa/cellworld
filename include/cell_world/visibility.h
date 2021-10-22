#pragma once
#include <cell_world/cell_group.h>
#include <cell_world/graph.h>
namespace cell_world{

    struct Visibility{
        static double angle_difference(double, double);
        static int direction(double, double);
        static double to_radians(double);
        static double to_degrees(double);
        static double normalize(double);
        static double normalize_degrees(double);
        static bool is_occluding(const Location &, const Location & , const Polygon &);
        static bool is_occluding(const Location &, double, double , const Polygon &);
    };

    struct Coordinates_visibility{
        static Graph create_graph(const Cell_group &, const Shape &, const Transformation &);
        static Graph invert(const Graph &);
    };

    struct Coordinates_visibility_cone{
        Coordinates_visibility_cone (const Graph &, double);
        Cell_group visible_cells(const Cell &, double);
        bool is_visible(const Cell &, double, const Cell &) const;
        const Graph &visibility;
        const double visual_angle;
    };

    struct Location_visibility{
        Location_visibility(const Cell_group &, const Shape &, const Transformation &);
        bool is_visible(const Location &, const Location &) const;
        Polygon_list occlusions;
    };
}