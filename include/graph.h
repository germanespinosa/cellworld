#pragma once
#include <core.h>
#include <cell_group.h>

namespace cell_world {
    struct Graph {
        Graph();
        explicit Graph(const Cell_group &);
        Cell_group &operator[](const Cell &);
        Cell_group &operator[](uint32_t);
        Cell_group get_shortest_path(const Cell &, const Cell &, bool shuffle = false);
        std::vector<Graph> get_sub_graphs();
        std::vector<Graph> get_sub_graphs(Cell_group &);
        std::vector<Graph> get_sub_graphs(Graph &);
        std::vector<Graph> get_sub_graphs(Graph &,Graph &);
        void add(const Cell &);
        void connect (const Cell &, const Cell_group &);
        double get_entropy();
        uint32_t size() const;
        Cell_group nodes;
        Graph operator !() const;
    private:
        std::vector<Cell_group> _connections;
    };

    struct Centrality {
        static std::vector<double> get_eigen_centrality(Graph&, uint32_t, double) ;
        static std::vector<double> get_eigen_centrality(Graph&) ;
        static std::vector<double> get_betweenness_centrality(Graph&, uint32_t) ;
        static std::vector<double> get_betweenness_centrality(Graph&) ;
    };
}