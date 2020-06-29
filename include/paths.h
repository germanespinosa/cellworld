#pragma once
#include <core.h>
#include <graph.h>

namespace cell_world {
    struct Paths {
        enum class Path_type{
            euclidean,
            shortest,
            mix
        };
        Paths(const Graph&, Path_type);
        Move get_move(const Cell &, const Cell &) const;
        bool save(const std::string &) const;
        bool save() const;
        Path_type type;
        bool operator==(const Paths&) const;
    //private:
        static std::string _type_string(Path_type);
        void _init(unsigned int);
        Paths() = default;
        std::string _name;
        Cell_group _cells;
        std::vector<std::vector<Move>> _next_move;
        const std::string _extension = ".path";
    friend class World;
    };
}