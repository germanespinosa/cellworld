#include <sub_world.h>
#include <string>
#include <algorithm>
#include <iostream>
using namespace std;
using namespace cell_world;
using namespace ge211;

Sub_worlds::Sub_worlds(const Cell_group &cells): _cells(cells)
{
}

uint32_t Sub_worlds::size() {
    return _sub_worlds.size();
}

#define SUB_WORLD_FIND_FIRST_NOT_FOUND for (; last_checked < _cells.size() && (cell_id = _cell_sub_world_index[last_checked]==Not_found ? last_checked : Not_found) == Not_found ; last_checked++)

void Sub_worlds::reset(const Cell_group &bridges, const Connections &connections) {
    gates.clear();
    _sub_worlds.clear();
    _cell_sub_world_index.clear();
    _gate_index.clear();
    for (uint32_t i = 0 ; i < _cells.size(); i++ ) {
        _cell_sub_world_index.push_back(_cells[i].occluded? Occluded :Not_found);
        _gate_index.push_back(Not_found);
    }
    for (uint32_t i = 0; i < bridges.size() ; i++) {
        _cell_sub_world_index[bridges[i].id] = Is_gate;
        gates.emplace_back(bridges[i]);
        _gate_index[bridges[i].id] = i;
    }
    int32_t cell_id = Not_found;
    uint32_t last_checked = 0;
    SUB_WORLD_FIND_FIRST_NOT_FOUND;
    while ( cell_id != Not_found ){
        Sub_world sub_world;
        uint32_t sub_world_id = _sub_worlds.size();
        uint32_t last_completed = 0;
        while ( cell_id != Not_found ) {
            sub_world.cells.add(_cells[cell_id]);
            _cell_sub_world_index[cell_id] = sub_world_id;
            cell_id = Not_found;
            for ( uint32_t j = last_completed; j < sub_world.cells.size() && cell_id == Not_found; j++ ){
                auto cell_cnn = sub_world.cells[j];
                auto c = connections[cell_cnn].connections;
                for( uint32_t k = 0; k < c.size() && cell_id == Not_found; k++ ){
                    auto cnn = c[k];
                    if (_cell_sub_world_index[cnn.id] == Not_found ) {
                        cell_id = cnn.id;
                    } else if (_cell_sub_world_index[cnn.id] == Is_gate){
                        sub_world.gate_cells.add(_cells[cnn.id]);
                    }
                }
                if (cell_id == Not_found) last_completed = j + 1;
            }
        }
        sub_world.cells += sub_world.gate_cells;
        _sub_worlds.push_back(sub_world);
        cell_id = Not_found;
        SUB_WORLD_FIND_FIRST_NOT_FOUND;
    }
    for (auto &sw:_sub_worlds)
        for (uint32_t i = 1; i < sw.gate_cells.size(); i++)
            for (uint32_t j = 0; j < i; j++) {
                gate_by_cell_id(sw.gate_cells[i].id).gate_connections.push_back({sw.gate_cells[j],sw});
                gate_by_cell_id(sw.gate_cells[j].id).gate_connections.push_back({sw.gate_cells[i],sw});
            }
}

int32_t Sub_worlds::get_sub_world_index(uint32_t cell_id) const {
    return _cell_sub_world_index[cell_id];
}

Cell_group Sub_worlds::find_bridges(const Connections &connections) {
    Cell_group candidates;
    // step 1 add all cells with at least 2 connections and less than 5
    for (uint32_t i = 0 ; i < _cells.size(); i++ ) {
        if (connections[i].connections.size()>=3 && connections[i].connections.size() <=4) candidates.add(_cells[i]);
    }
    reset(candidates, connections);
    // step 2 remove all the candidates not connecting to any world
    Cell_group candidates2;
    for (uint32_t i = 0; i < candidates.size(); i++) {
        uint32_t cell_id = candidates[i].id;
        if (!gate_by_cell_id(cell_id).gate_connections.empty()) candidates2.add(_cells[cell_id]);
    }
    reset(candidates2, connections);
    // step 3 remove all the redundant candidates
    uint32_t target_size = _sub_worlds.size();
    Cell_group candidates3 = candidates2;
    for (uint32_t i = 0; i < candidates2.size(); i++) {
        uint32_t cell_id = candidates2[i].id;
        candidates3.remove(_cells[cell_id]);
        reset( candidates3, connections);
        if (_sub_worlds.size() < target_size) candidates3.add(_cells[cell_id]);
    }
    // step 4 remove all candidates connecting to only one world
    Cell_group candidates4 = candidates3;
    for (uint32_t i = 0; i < candidates3.size(); i++) {
        uint32_t cell_id = candidates3[i].id;
        if (gate_by_cell_id(cell_id).gate_connections.size()==1) candidates4.remove(_cells[cell_id]);
    }
    return candidates4;
}

Gate &Sub_worlds::gate_by_cell_id(uint32_t cell_id) {
    return gates[_gate_index[cell_id]];
}

Gate::Gate(const Cell &cell) :
        cell (cell){}

bool Sub_world::save(const std::string& sub_world_name) {
    cells.save(sub_world_name+"-cells.sw");
    gate_cells.save(sub_world_name+"-gates.sw");
    return false;
}

Sub_worlds::Sub_worlds(const Cell_group &cells, const Cell_group &bridges, const Connections &connections) : _cells(cells){
    reset (bridges, connections);
}

const Sub_world &Sub_worlds::operator[](uint32_t index) const {
    return _sub_worlds[index];
}

