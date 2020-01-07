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