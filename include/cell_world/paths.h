#pragma once
#include <cell_world/core.h>
#include <cell_world/graph.h>

namespace cell_world {
    using Path_builder = Move_list;

    struct Paths : json_cpp::Json_object {
        explicit Paths(const Graph&);
        Paths(const Graph&, Move_list);
        Move get_move(const Cell &, const Cell &) const;
        bool set_move(const Cell &, const Cell &, const Move &);
        bool operator==(const Paths&) const;
        Json_object_members({
                         Add_member(moves);
        })
        const Cell_group cells;
        Move_list moves;
        static Paths get_euclidean(const Graph &);
        static Paths get_manhattan(const Graph &);
        static Paths get_astar(const Graph &);
    private:
        int _index(const Cell &, const Cell &) const;
    };
}