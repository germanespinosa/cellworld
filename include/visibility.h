#pragma once
#include <cell_group.h>
#include <graph.h>
namespace cell_world{
    struct Visibility{
        static Graph create_graph(const Cell_group &);
    };
}