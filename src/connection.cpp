//
// Created by german on 12/14/19.
//

#include <connection.h>
#include <iostream>
using namespace std;
using namespace cellworld;

World_connections::World_connections(World &w, std::vector<Coordinates> patterns) {
    for (unsigned int i=0; i < w.size(); i++){
        vector<uint32_t> cell_conns;
        for (unsigned int j=0; j < patterns.size(); j++) {
            Coordinates c = w[i].coordinates + patterns[j];
            int32_t id = w.find(c);
            if (id >=0 && !w[id].occluded){
                cell_conns.push_back(id);
            }
        }
        _connections.push_back(cell_conns);
    }
}

std::vector<uint32_t> &World_connections::cell_connections(uint32_t cell_id) {
    return _connections[cell_id];
}

