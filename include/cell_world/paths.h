#pragma once
#include <cell_world/core.h>
#include <cell_world/graph.h>

namespace cell_world {
    struct Path_builder : json_cpp::Json_object {
        Move_list moves;
        json_cpp::Json_vector<int> steps;
        Json_object_members({
            Add_member(moves);
            Add_member(steps);
        })
    };

    struct Paths : json_cpp::Json_object {
        explicit Paths(const Graph&);
        Paths(const Paths &);
        Paths(const Graph&, Move_list );
        Paths(const Graph&, const Path_builder &);
        Move get_move(const Cell &, const Cell &) const;
        int get_steps(const Cell &, const Cell &) const;
        Cell_group get_path(const Cell &, const Cell &);
        Move_list get_moves(const Cell &, const Cell &);
        bool set_move(const Cell &, const Cell &, const Move &);
        bool operator==(const Paths&) const;
        Json_object_members({
            Add_member(moves);
            Add_member(steps);
        })
        Cell_group cells;
        Move_list moves;
        json_cpp::Json_vector<int> steps;
        static Paths get_euclidean(const Graph &);
        static Paths get_manhattan(const Graph &);
        static Paths get_astar(const Graph &);
        Paths& operator =(const Paths&);
    private:
        int _index(const Cell &, const Cell &) const;
        void _compute_steps();
    };
}