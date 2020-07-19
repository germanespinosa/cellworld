#pragma once
#include <cell_world/core.h>
#include <cell_world/graph.h>

namespace cell_world {
    struct Paths : json_cpp::Json_object {
        enum Path_type{
            euclidean,
            shortest,
            mix
        };
        Paths(const Graph&, Path_type);
        Move get_move(const Cell &, const Cell &) const;
        Path_type type;
        bool operator==(const Paths&) const;
        Json_set_builder({
            Json_add_member(type,true);
            Json_add_member(_name,true);
            Json_add_member(_next_move,true);
        })
    private:
        static std::string _type_string(Path_type);
        void _init(unsigned int);
        Paths() = default;
        std::string _name;
        Cell_group _cells;
        json_cpp::Json_vector<Move_list> _next_move;
        const std::string _extension = ".path";
    friend class World;
    };
}