#pragma once
#include <cell_world/core.h>
#include <cell_world/graph.h>

namespace cell_world {
    struct Paths : json_cpp::Json_object {
        Paths(const Graph&);
        Move get_move(const Cell &, const Cell &) const;
        bool set_move(const Cell &, const Cell &, const Move &);
        bool operator==(const Paths&) const;
        Json_object_members({
                         Add_member(cells,true);
                         Add_member(next_move,true);
        })
        const Cell_group cells;
        Move_list next_move;
    private:
        int _index(const Cell &, const Cell &) const;
    };
    struct Path_builder {
        static Paths get_euclidean(const Graph &);
        static Paths get_manhattan(const Graph &);
        static Paths get_shortest(const Graph &);
        static Paths load_paths(const Graph &);
    };
}