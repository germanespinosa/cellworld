#pragma once
#include <cell_world/cell_group.h>

namespace cell_world {

    using Graph_builder = json_cpp::Json_vector<json_cpp::Json_vector<unsigned int>>;

    struct Graph : json_cpp::Json_vector<Cell_group>{
        Graph();
        explicit Graph(const Cell_group &);
        Graph(const cell_world::Graph&) = default;
        bool add(const Cell &);
        bool add(const Cell_group &);
        void connect (const Cell &, const Cell_group &);
        void connect (const Graph &);
        std::vector<Coordinates> get_connectors (const Cell &);
        float get_entropy();
        Graph invert() const;
        const Cell_group &operator[](const Cell &) const;
        Cell_group &operator[](const Cell &);
        const Cell_group &operator[](unsigned int) const;
        Cell_group &operator[](unsigned int);
        Graph operator !() const;
        Graph &operator = (const Graph &);
        bool operator == (const Graph &) const;
        Cell_group cells;
        Cell_group get_shortest_path(const Cell &, const Cell &, bool shuffle = false) const;
        json_cpp::Json_vector<float> get_centrality(int depth=1);
        json_cpp::Json_vector<Graph> get_sub_graphs();
        json_cpp::Json_vector<Graph> get_sub_graphs(Cell_group &);
        json_cpp::Json_vector<Graph> get_sub_graphs(Graph &);
        json_cpp::Json_vector<Graph> get_sub_graphs(Graph &,Graph &);
        bool is_connected(const Cell &,const Cell&) const;
    };
}