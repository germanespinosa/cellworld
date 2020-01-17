#pragma once

#include <cinttypes>
#include <string>
#include <vector>
#include <cell_group.h>
namespace cell_world {

    struct Connection_pattern{
        Connection_pattern();
        explicit Connection_pattern(std::vector<Coordinates>);
        std::vector<Coordinates> pattern;
        std::vector<Coordinates> get_candidates(Coordinates) const;
        const Coordinates &operator [](uint32_t);
        uint32_t size();
        bool load(const std::string&);
        bool save(const std::string&) const;
    private:
        const std::string _extension = ".pattern";
    };

    struct Connection {
        explicit Connection (const Cell &);
        const Cell &cell;
        double eigen_centrality{};
        Cell_group connections;
    private:
        const std::string _extension = ".connection";
    };

    struct Connections {
        Connections() = default;
        explicit Connections(const Cell_group &);
        explicit Connections(const Cell_group &, const Connection_pattern &);
        void reset(const Connection_pattern &);
        bool add(const Cell &, const Cell &);
        bool save(const std::string &) const;
        const Connection &operator [](uint32_t) const;
        const Connection &operator [](const Cell &) const;
        bool process_eigen_centrality();
        bool process_eigen_centrality(uint32_t, double);
        void clear();
        uint32_t size() const;
        Cell_group cells;
    private:
        std::vector<Connection> _connections;
        std::vector<int32_t> _id_index;
        const std::string _extension = ".connections";
    };
}