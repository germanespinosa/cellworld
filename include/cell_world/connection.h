#pragma once
#include <cell_world/core.h>
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
    };

    struct World_configuration : json_cpp::Json_object{
        Cell_descriptor cell_descriptor;
        Coordinates_list cell_coordinates;
        Connection_pattern connection_pattern;
        Json_object_members({
                                Add_member(cell_coordinates);
                                Add_member(connection_pattern);
                            })
    };
}