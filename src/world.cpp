#include <world.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <utility>

using namespace std;
using namespace cell_world;
using namespace ge211;

bool World::add(Cell cell){
    cell.id = _cells.size();
    vector<double> distances;
    for (unsigned i=0; i<_cells.size() ; i ++){
        double dist = distance(cell,_cells[i]);
        distances.push_back(dist);
        _distances[i].push_back(dist);
    }
    distances.push_back(0);
    _distances.push_back(distances);
    _cells.emplace_back(cell);
    return true;
}

bool World::load(const std::string& file_path){
    _file_name = file_path;
    _cells.clear();
    std::ifstream file;
    file.open(file_path.c_str());
    string line;
    while (getline(file, line)){
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

bool World::save(const std::string& file_path) const{
    std::ofstream file;
    file.open(file_path.c_str());
    for (const auto & cell : _cells){
        file
            << (int16_t)cell.coordinates.x << " "
            << (int16_t)cell.coordinates.y << " "
            << cell.location.x << " "
            << cell.location.y << " "
            << cell.value << " "
            << cell.occluded ;
        file << std::endl;
    }
    return true;
}

uint32_t World::size() const{
    return _cells.size();
}

const Cell &World::operator[](const uint32_t& id) const{
    return _cells[id];
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
    _cells[id].occluded = occluded;
}

void World::set_value(uint32_t id, double value) {
    _cells[id].value = value;
}

bool World::save() const {
    return save(name + ".map");
}

bool World::load() {
    return load(name + ".map");
}

World::World(std::string name) : name (std::move(name)){
}

Cell_group World::create_cell_group() const{
    Cell_group cg;
    for (uint32_t i = 0; i < _cells.size(); i++) cg.add(_cells[i]);
    return cg;
}

Cell_group World::create_cell_group(const std::vector<uint32_t>& cell_ids) const{
    Cell_group cg;
    for (auto id : cell_ids) cg.add(_cells[id]);
    return cg;
}

Cell_group World::create_cell_group(std::string file_path) const{
    Cell_group cg;
    std::ifstream file;
    file.open(file_path.c_str());
    string line;
    while (getline(file, line)){
        istringstream ss(line);
        int32_t cell_id;
        Cell cell;
        ss >> cell_id;
        cg.add(_cells[cell_id]);
    }
    return cg;
}
