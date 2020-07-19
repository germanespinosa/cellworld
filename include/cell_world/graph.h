#pragma once
#include <cell_world/core.h>
#include <cell_world/cell_group.h>

namespace cell_world {
    struct Graph {
        Graph();
        explicit Graph(const Cell_group &);
        Graph(const cell_world::Graph&) = default;
        Cell_group &operator[](const Cell &);
        const Cell_group &operator[](const Cell &) const;
        Cell_group &operator[](unsigned int);
        const Cell_group &operator[](unsigned int) const;
        Cell_group get_shortest_path(const Cell &, const Cell &, bool shuffle = false) const;
        std::vector<Graph> get_sub_graphs();
        std::vector<Graph> get_sub_graphs(Cell_group &);
        std::vector<Graph> get_sub_graphs(Graph &);
        std::vector<Graph> get_sub_graphs(Graph &,Graph &);
        bool add(const Cell &);
        bool add(const Cell_group &);
        bool remove(const Cell &);
        void connect (const Cell &, const Cell_group &);
        void connect (Graph &);
        std::vector<Coordinates> get_connectors (const Cell &);
        double get_entropy();
        void clear();
        unsigned int size() const;
        Graph operator !() const;
        Graph &operator =(const Graph &);
        bool operator == (const Graph &) const;
        Cell_group nodes;
        Graph invert() const;
        friend std::ostream& operator << (std::ostream& , const Graph& );
    private:
        std::vector<Cell_group> _connections;
    };

    struct Centrality {
        static std::vector<double> get_eigen_centrality(Graph&, unsigned int, double) ;
        static std::vector<double> get_eigen_centrality(Graph&) ;
        static std::vector<double> get_betweenness_centrality(Graph&, unsigned int) ;
        static std::vector<double> get_betweenness_centrality(Graph&) ;
    };
}