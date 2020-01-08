#include <world.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <utility>

using namespace std;
using namespace cell_world;
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

bool World::load(const std::string& file_path){
    _file_name = file_path;
    cells.clear();
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
    for (const auto & cell : cells){
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
            for (auto j : pattern) {
                Coordinates c = cells[source].coordinates + j;
                int32_t destination = this->find(c);
                if (destination != Not_found && !cells[destination].occluded) {
                    connections.add(source,destination);
                }
            }
        }
    }
}

World::World(std::string name) : name (std::move(name)){
    for (auto & i : _map) for (int & j : i) j = Not_found;
}


