#pragma once
#include <core.h>
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
        Connection_pattern &operator =(const Connection_pattern &);
        bool load_from_string(const std::string&);
        std::string save_to_string() const;
    private:
        const std::string _extension = ".pattern";
    };

    struct Connection {
        explicit Connection (const Cell &);
        const Cell &cell;
        double eigen_centrality{};
        double betweenness_centrality{};
        Cell_group connections;
    private:
        const std::string _extension = ".connection";
    };

    struct Connections {
        Connections() = default;
        explicit Connections(const Cell_group &);
        explicit Connections(const Cell_group &, const Connection_pattern &);
        void reset(const Connection_pattern &);
        bool save(const std::string &) const;
        const Connection &operator [](uint32_t) const;
        const Connection &operator [](const Cell &) const;
        bool process_eigen_centrality();
        bool process_betweenness_centrality(uint32_t);
        bool process_betweenness_centrality();
        bool process_eigen_centrality(uint32_t, double);
        uint32_t size() const;
        Cell_group cells;
        Cell_group shortest_path(const Cell&,const Cell &);
    private:
        bool _add(const Cell &, const Cell &);
        void _clear();
        std::vector<Connection> _connections;
        std::vector<int32_t> _id_index;
        const std::string _extension = ".connections";
    };
}