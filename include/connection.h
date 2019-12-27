//
// Created by german on 12/14/19.
//

#ifndef CELLWORLD_CONNECTION_H
#define CELLWORLD_CONNECTION_H
#include "world.h"
namespace cellworld {
    struct World_connections {
        World_connections(World &, std::vector<Coordinates>);
        std::vector<uint32_t> &cell_connections(uint32_t);
        std::vector<double> eigen_centrality;
        bool process_eigen_centrality(uint32_t, double);
        bool process_eigen_centrality();
    private:
        std::vector<std::vector<uint32_t>> _connections;
    };

}
#endif //CELLWORLD_CONNECTION_H
