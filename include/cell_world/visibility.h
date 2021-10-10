#pragma once
#include <cell_world/cell_group.h>
#include <cell_world/graph.h>
namespace cell_world{
    struct Visibility{
        static Graph create_graph(const Cell_group &, const Cell_descriptor &);
        static Graph invert(const Graph &);
    };
    struct Visibility_cone{
        Visibility_cone (const Graph &, double);
        Cell_group visible_cells(const Cell &, double);
        bool is_visible(const Cell &, double, const Cell &);
        static double angle_difference(double, double);
        static int direction(double, double);
        static double to_radians(double);
        static double to_degrees(double);
        static double normalize(double);
        static double normalize_degrees(double);
        const Graph &visibility;
        const double visual_angle;
    };
}