#include <sub_world.h>
#include <string>
#include <algorithm>

using namespace std;
using namespace cellworld;
using namespace ge211;

Sub_worlds::Sub_worlds()
{
    _size = 0;
}

uint32_t Sub_worlds::size() {
    return _size;
}

#define SUB_WORLD_FIND_FIRST_NOT_FOUND for (; last_checked < world.size() && (cell_id = _cell_sub_world_index[last_checked]==Not_found ? last_checked : Not_found) == Not_found ; last_checked++)

void Sub_worlds::reset(const World &world, const Cell_group &bridges, const Connections &connections) {
    gates.clear();
    _sub_worlds.clear();
    _cell_sub_world_index.clear();
    _gate_index.clear();
    for (uint32_t i = 0 ; i < world.size(); i++ ) {
        _cell_sub_world_index.push_back(world[i].occluded? Occluded :Not_found);
        _gate_index.push_back(Not_found);
    }
    for (uint32_t i = 0; i < bridges.size() ; i++) {
        _cell_sub_world_index[bridges[i].id] = Is_gate;
        gates.emplace_back(bridges[i].id);
        _gate_index[bridges[i].id] = i;
    }
    int32_t cell_id = Not_found;
    uint32_t last_checked = 0;
    SUB_WORLD_FIND_FIRST_NOT_FOUND;
    while ( cell_id != Not_found ){
        Sub_world sub_world(world);
        uint32_t sub_world_id = _sub_worlds.size();
        uint32_t last_completed = 0;
        while ( cell_id != Not_found ) {
            sub_world.cells.add(cell_id);
            _cell_sub_world_index[cell_id] = sub_world_id;
            cell_id = Not_found;
            for ( uint32_t j = last_completed; j < sub_world.cells.size() && cell_id == Not_found; j++ ){
                uint32_t cell_id_cnn = sub_world.cells[j].id;
                for( uint32_t k = 0; k < connections[cell_id_cnn].size() && cell_id == Not_found; k++ ){
                    uint32_t cnn = connections[sub_world.cells[j].id][k];
                    if (_cell_sub_world_index[cnn] == Not_found ) {
                        cell_id = cnn;
                    } else if (_cell_sub_world_index[cnn] == Is_gate){
                        uint32_t gate_id = _gate_index[cnn];
                        gates[gate_id].connect(sub_world_id);
                        sub_world.add_gate(gate_id);
                    }
                }
                if (cell_id == Not_found) last_completed = j + 1;
            }
        }
        _sub_worlds.push_back(sub_world);
        cell_id = Not_found;
        SUB_WORLD_FIND_FIRST_NOT_FOUND;
    }
    _size =  _sub_worlds.size();
}

int32_t Sub_worlds::get_sub_world_index(uint32_t cell_id) const {
    return _cell_sub_world_index[cell_id];
}

bool Sub_worlds::get_cells(Cell_group &cg, uint32_t sub_world_id) const {
    cg.clear();
    if (sub_world_id>=_size) return false;
    for (uint32_t i=0;i<_cell_sub_world_index.size();i++) if (_cell_sub_world_index[i] == (int32_t)sub_world_id) cg.add(i);
    return true;
}

Cell_group Sub_worlds::find_bridges(const World &world, const Connections &connections) {
    Cell_group candidates(world);
    // step 1 add all cells with at least 2 connections and less than 5
    for (uint32_t i = 0 ; i < world.size(); i++ ) {
        if (connections[i].size()>=3 && connections[i].size() <=4) candidates.add(i);
    }
    reset(world, candidates, connections);
    // step 2 remove all the candidates not connecting to any world
    Cell_group candidates2(world);
    for (uint32_t i = 0; i < candidates.size(); i++) {
        uint32_t cell_id = candidates[i].id;
        if (gate_by_cell_id(cell_id).sub_world_ids.size()>0) candidates2.add(cell_id);
    }
    reset(world, candidates2, connections);
    // step 3 remove all the redundant candidates
    uint32_t target_size = _size;
    Cell_group candidates3 = candidates2;
    for (uint32_t i = 0; i < candidates2.size(); i++) {
        uint32_t cell_id = candidates2[i].id;
        candidates3.remove(cell_id);
        reset(world, candidates3, connections);
        if (_size < target_size) candidates3.add(cell_id);
    }
    // step 4 remove all candidates connecting to only one world
    Cell_group candidates4 = candidates3;
    for (uint32_t i = 0; i < candidates3.size(); i++) {
        uint32_t cell_id = candidates3[i].id;
        if (gate_by_cell_id(cell_id).sub_world_ids.size()==1) candidates4.remove(cell_id);
    }
    return candidates4;
}

Gate &Sub_worlds::gate_by_cell_id(uint32_t cell_id) {
    return gates[_gate_index[cell_id]];
}

void Sub_worlds::reset_connections() {
    for (uint32_t i = 0; i < gates.size() ; i++) {
        Gate &g = gates[i];
        for (uint32_t j = 0; j < g.sub_world_ids.size() ; j++ ){
            uint32_t world_id = g.sub_world_ids[j];
            for (uint32_t k = 0; k < gates.size() ; k++) {
                if (i!=k && gates[k].is_connected(world_id)) g.gate_connections.push_back({gates[k].cell_id,world_id});
            }
        }
    }
}

Gate::Gate(uint32_t cell_id) :
        cell_id (cell_id){}

bool Gate::connect(uint32_t world_id) {
    if (is_connected(world_id)) return false;
    sub_world_ids.push_back(world_id);
    return true;
}

bool Gate::is_connected(uint32_t world_id) {
    for(uint32_t i = 0; i<sub_world_ids.size(); i++) if (sub_world_ids[i]==world_id) return true;
    return false;
}

Sub_world::Sub_world(const World &world):
        cells(world){}

bool Sub_world::is_connected(uint32_t gate_id) {
    for(uint32_t i = 0; i<_gate_ids.size(); i++) if (_gate_ids[i]==gate_id) return true;
    return false;
}

bool Sub_world::add_gate(uint32_t gate_id) {
    if (is_connected(gate_id)) return false;
    _gate_ids.push_back(gate_id);
    return true;
}

Sub_worlds::Sub_worlds(const World &world, const Cell_group &bridges, const Connections &connections) {
    _size = 0;
    reset (world, bridges, connections);
}
