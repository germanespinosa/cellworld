#include <world.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;
using namespace cellworld;
using namespace ge211;

bool Cell::operator == (const Cell& c) const {
    return (id == c.id &&
            coordinates == c.coordinates &&
            location == c.location &&
            occluded == c.occluded);
}
    

Cell::Cell (uint32_t id, Coordinates coordinates, Basic_position<double> location, double value, bool occluded)
{
    this->id = id;
    this->location = location;
    this->coordinates = coordinates;
    this->value = value;
    this->occluded=occluded;
}

Cell::Cell(){
    this->id = 0;
    this->location = {0,0};
    this->coordinates = {0,0};
    this->occluded= false;
    this->value=0;
}

bool World::add(Cell cell){
    cell.id = cells.size();
    vector<double> distances;
    for (unsigned i=0; i<cells.size() ; i ++){
        double dist = distance(cell,cells[i]);
        distances.push_back(dist);
        _distances[i].push_back(dist);
    }
    distances.push_back(0);
    _distances.push_back(distances);
    cells.push_back(cell);
    uint32_t i = ( cell.coordinates.x + 128 );
    uint32_t j = ( cell.coordinates.y + 128 );
    _map[i][j] = cell.id;
    return true;
}

bool World::load(const std::string filepath){
    _file_name = filepath;
    cells.clear();
    std::ifstream ifile;
    ifile.open(filepath.c_str());
    string line;
    while (getline(ifile, line)){
        istringstream ss(line);
        int16_t cx,cy;
        Cell cell;
        ss >> cx;
        ss >> cy;
        ss >> cell.location.x;
        ss >> cell.location.y;
        ss >> cell.value;
        ss >> cell.occluded;
        cell.coordinates.x = cx;
        cell.coordinates.y = cy;
        if (!add(cell)) return false;
    }
    return true;
}

bool World::save(const std::string filepath) const{
    std::ofstream  ofile;
    ofile.open(filepath.c_str());
    for (unsigned int i=0;i<cells.size();i++){
        ofile
        << (int16_t)cells[i].coordinates.x << " "
        << (int16_t)cells[i].coordinates.y << " "
        << cells[i].location.x << " "
        << cells[i].location.y << " "
        << cells[i].value << " "
        << cells[i].occluded ;
       ofile << std::endl;
    }
    return true;
}

uint32_t World::size() const{
    return cells.size();
}

int32_t World::find (const Coordinates& coordinates) const{
    uint32_t i = ( coordinates.x + 128 );
    uint32_t j = ( coordinates.y + 128 );
    return _map[i][j];
}

const Cell &World::operator[](const uint32_t& id) const{
    return cells[id];
}

const Cell &World::operator[](const Coordinates& coordinates) const{
    return cells[find(coordinates)];
}

double World::distance(const Cell &c0, const Cell &c1) const {
    return sqrt(pow(c1.location.y-c0.location.y,2) + pow(c1.location.x-c0.location.x,2));
}
double World::distance(const Cell  &c0, const Cell &c1, const Cell &c2) const {
    return abs((c2.location.y-c1.location.y) * c0.location.x-(c2.location.x-c1.location.x) *c0.location.y+c2.location.x*c1.location.y-c2.location.y * c1.location.x) / sqrt(pow(c2.location.y-c1.location.y,2)+pow(c2.location.x-c1.location.x,2));
}

double World::distance(const uint32_t s, const uint32_t d) const {
    return _distances[s][d];
}

void World::set_occlusion(uint32_t id, bool occluded) {
    cells[id].occluded = occluded;
}

void World::set_value(uint32_t id, double value) {
    cells[id].value = value;
}

bool World::save() const {
    return save(name + ".map");
}

bool World::load() {
    return load(name + ".map");
}

void World::get_connections(Connections & connections , const std::vector<Coordinates> &pattern) const {
    connections.clear();
    for (unsigned int source = 0; source < cells.size(); source++){
        if ( !cells[source].occluded ) {
            for ( unsigned int j = 0; j < pattern.size(); j++ ) {
                Coordinates c = cells[source].coordinates + pattern[j];
                int32_t destination = this->find(c);
                if (destination >= 0 && !cells[destination].occluded) {
                    connections.add(source,destination);
                }
            }
        }
    }
}

World::World(std::string name) : name (name){
    for (unsigned int i = 0; i < 256; i++) for (unsigned int j = 0; j < 256; j++)  _map[i][j] = -1;
}

Cell_group::Cell_group(const World &world):
_world(world){

}

bool Cell_group::load( const std::string &file_path) {
    _file_name = file_path;
    _cell_ids.clear();
    std::ifstream ifile;
    ifile.open(file_path.c_str());
    string line;
    while (getline(ifile, line)){
        istringstream ss(line);
        int32_t cell_id;
        Cell cell;
        ss >> cell_id;
        if (!add(cell_id)) return false;
    }
    return true;
}

bool Cell_group::add(uint32_t cell_id) {
    if (cell_id >= _world.size()) return false;
    if (std::find(_cell_ids.begin(),_cell_ids.end(),cell_id) == _cell_ids.end()){
        _cell_ids.push_back(cell_id);
        return true;
    }
    return false;
}

bool Cell_group::remove(uint32_t cell_id) {
    std::vector<uint32_t>::iterator it;
    if ((it = std::find(_cell_ids.begin(),_cell_ids.end(),cell_id)) != _cell_ids.end()){
        _cell_ids.erase(it);
        return true;
    }
    return false;
}

bool Cell_group::toggle(uint32_t cell_id) {
    if (!add(cell_id)) return remove(cell_id);
    return true;
}

uint32_t Cell_group::size() const {
    return _cell_ids.size();
}

const Cell &Cell_group::operator[](uint32_t index) const {
    return _get_cell(index);
}

bool Cell_group::save(const std::string &file_path) const {
    std::ofstream ofile;
    ofile.open(file_path.c_str());
    for (unsigned int i = 0 ; i < _cell_ids.size() ; i++) {
        ofile << _cell_ids[i];
        ofile << std::endl;
    }
    return true;
}

bool Cell_group::load() {
    return load(_file_name);
}

bool Cell_group::save() const{
    return save(_file_name);
}

void Cell_group::clear() {
    _cell_ids.clear();
}

int32_t Cell_group::find(Coordinates coordinates) const {
    int32_t  index = _world.find(coordinates);
    if (index < 0) return index;
    for (uint32_t i = 0; i < _cell_ids.size(); i++){
        if (_cell_ids[i] == (uint32_t)index) return i;
    }
    return -1;
}

int32_t Cell_group::find(uint32_t id) const {
    for (uint32_t i = 0; i < _cell_ids.size(); i++){
        if (_world[_cell_ids[i]].id == id) return i;
    }
    return -1;
}


void Cell_group::get_connections(Connections &connections, const std::vector<Coordinates> pattern) const {
    for (uint32_t source=0; source < _cell_ids.size(); source++){
        const Cell & source_cell = _get_cell(_cell_ids[source]);
        if (!source_cell.occluded) {
            for (uint32_t  j = 0; j < pattern.size(); j++) {
                Coordinates c = source_cell.coordinates + pattern[j];
                int32_t destination = find(c);
                if (destination >= 0) {
                    const Cell &destination_cell = _get_cell(destination);
                    if (!destination_cell.occluded) connections.add(source,destination);
                }
            }
        }
    }
}

const Cell &Cell_group::_get_cell(uint32_t index) const {
    return _world[_cell_ids[index]];
}

bool Cell_group::contains(uint32_t id) const {
    for (uint32_t i = 0; i<_cell_ids.size(); i++) if (_cell_ids[i]==id) return true;
    return false;
}

Cell_group &Cell_group::operator = (Cell_group &cg) {
    clear();
    for (uint32_t i = 0; i < cg.size() ; i++) _cell_ids.push_back(cg[i].id);
    return cg;
}

Sub_worlds::Sub_worlds()
{
    _size = 0;
}

uint32_t Sub_worlds::size() {
    return _size;
}

void Sub_worlds::reset(const World &world, const Cell_group &bridges, const Connections &connections) {
    _sub_worlds.clear();
    _cell_sub_world_index.clear();
    _gate_index.clear();
    for (uint32_t i = 0 ; i < world.size(); i++ ) {
        _cell_sub_world_index.push_back(world[i].occluded? occluded :Not_found);
        _gate_index.push_back(Not_found);
    }
    for (uint32_t i = 0; i < bridges.size() ; i++) {
        _cell_sub_world_index[bridges[i].id]=is_gate;
        gates.emplace_back(bridges[i].id);
        _gate_index[bridges[i].id] = i;
    }
    int32_t cell_id = Not_found;
    uint32_t last_checked = 0;
    for (; last_checked < world.size() && cell_id == Not_found ; last_checked++) if (_cell_sub_world_index[last_checked]==Not_found) cell_id = last_checked;
    while ( cell_id != Not_found ){
        Sub_world sub_world(world);
        uint32_t last_completed = 0;
        while ( cell_id != Not_found ) {
            sub_world.cells.add(cell_id);
            _cell_sub_world_index[cell_id] =_sub_worlds.size();
            cell_id = Not_found;
            for ( uint32_t j = last_completed; j < sub_world.cells.size() && cell_id == Not_found; j++){
                for( uint32_t k = 0; k < connections[sub_world.cells[j].id].size() && cell_id == Not_found; k++ ){
                    if (_cell_sub_world_index[connections[sub_world.cells[j].id][k]] == Not_found ) {
                        cell_id = connections[sub_world.cells[j].id][k];
                    }
                }
                if (cell_id == Not_found) last_completed = j + 1;
            }
        }
        _sub_worlds.push_back(sub_world);
        cell_id = Not_found;
        for (; last_checked < world.size() && cell_id == Not_found ; last_checked++) if (_cell_sub_world_index[last_checked]==Not_found) cell_id = last_checked;
    }
    _size =  _sub_worlds.size();
    for (uint32_t i = 0; i < gates.size() ; i++){
        Gate &g = gates[i];
        for (uint32_t j = 0; j < connections[g._cell_id].size() ; j++) {
            int32_t sub_world_index = _cell_sub_world_index[connections[g._cell_id][j]];
            if (sub_world_index >= 0) {
                g.connect(sub_world_index);
                _sub_worlds[sub_world_index].add_gate(j);
            }
        }
    }
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
        if (gate_by_cell_id(cell_id)._sub_world_ids.size()>0) candidates2.add(cell_id);
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
    return candidates3;
}

Gate &Sub_worlds::gate_by_cell_id(uint32_t cell_id) {
    return gates[_gate_index[cell_id]];
}

void Sub_worlds::reset_connections() {
    for (uint32_t i = 0; i < gates.size() ; i++) {
        Gate &g = gates[i];
        for (uint32_t j = 0; j < g._sub_world_ids.size() ; j++ ){
            uint32_t world_id = g._sub_world_ids[j];
            for (uint32_t k = 0; k < gates.size() ; k++) {
                if (i!=k && gates[k].is_connected(world_id)) g.gate_connections.push_back({gates[k]._cell_id,world_id});
            }
        }
    }
}

Gate::Gate(uint32_t cell_id) :
_cell_id (cell_id){}

bool Gate::connect(uint32_t world_id) {
    if (is_connected(world_id)) return false;
    _sub_world_ids.push_back(world_id);
    return true;
}

bool Gate::is_connected(uint32_t world_id) {
    for(uint32_t i = 0; i<_sub_world_ids.size(); i++) if (_sub_world_ids[i]==world_id) return true;
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
