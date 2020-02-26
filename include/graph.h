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
        bool add(const Cell &);
        bool remove(const Cell &);
        void connect (const Cell &, const Cell_group &);
        void connect (Graph &);
        double get_entropy();
        void clear();
        uint32_t size() const;
        Graph operator !() const;
        Graph &operator =(const Graph &);
        bool operator == (const Graph &) const;
        Cell_group nodes;
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