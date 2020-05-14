#pragma once
#include <core.h>
#include <graph.h>

namespace cell_world {
    struct Path {
        enum class Distance_type{
            euclidean,
            a_star
        };
        Path(const Graph&, Distance_type);
        Move get_move(const Cell &, const Cell &) const;
        bool save(const std::string &) const;
    private:
        Cell_group _cells;
        std::vector<std::vector<Move>> _next_move;
        const std::string _extension = ".path";
    };
}