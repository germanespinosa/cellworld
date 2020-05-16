#pragma once
#include <core.h>
#include <cell_group.h>
#include <graph.h>
namespace cell_world {

    struct Connection_pattern{
        Connection_pattern();
        Connection_pattern(const cell_world::Connection_pattern&) = default;
        explicit Connection_pattern(std::vector<Move>);
        std::vector<Move> pattern;
        std::vector<Move> get_candidates(Move) const;
        const Coordinates &operator [](uint32_t);
        uint32_t size() const;
        bool load(const std::string&);
        bool save(const std::string&) const;
        Graph get_graph(const Cell_group &) const;
        Connection_pattern &operator =(const Connection_pattern &);
        bool load_from_string(const std::string&);
        std::string save_to_string() const;
        static Connection_pattern get_pattern(Cell, Cell_group);
        Connection_pattern &operator=(const std::string &);
        Move random_move();
    private:
        const std::string _extension = ".pattern";
    };
}