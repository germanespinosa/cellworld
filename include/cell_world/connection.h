#pragma once
#include <cell_world/cell_group.h>
#include <cell_world/graph.h>

namespace cell_world {

    struct Connection_pattern: Coordinates_list{
        Connection_pattern();
        Connection_pattern(const cell_world::Connection_pattern&) = default;
        Coordinates_list get_candidates(Coordinates) const;
        Graph get_graph(const Cell_group &) const;
        Connection_pattern &operator =(const Connection_pattern &);
        static Connection_pattern get_pattern(Cell, Cell_group);
        Connection_pattern &operator=(const std::string &);
        Move random_move() const;
        Json_vector<Connection_pattern> get_pairs();
    };
}