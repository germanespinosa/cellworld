#pragma once
#include <cell_world/cell_group.h>
#include <cell_world/graph.h>
namespace cell_world{
    struct Visibility{
        static Graph create_graph(const Cell_group &);
        static Graph invert(const Graph &);
    };
}