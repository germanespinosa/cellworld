#include <cell_group.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>

using namespace std;
using namespace cell_world;
using namespace ge211;

uint32_t Cell_group::size() const {
    return _cells.size();
}

const Cell &Cell_group::operator[](uint32_t index) const {
    return _get_cell(index);
}

bool Cell_group::save(const std::string &name) const {
    string file_path = name + _extension;
    std::ofstream file;
    file.open(file_path.c_str());
    for (auto _cell_ref : _cells) {
        file << _cell_ref.get().id;
        file << std::endl;
    }
    return true;
}

bool Cell_group::save() const{
    return save(_file_name);
}

void Cell_group::clear() {
    _cells.clear();
    _id_index.clear();
}

int32_t Cell_group::find(uint32_t cell_id) const {
    if (cell_id>=_id_index.size()) return Not_found;
    return _id_index[cell_id];
}

int32_t Cell_group::find(const Cell &cell) const {
    if (cell.id>=_id_index.size()) return Not_found;
    return _id_index[cell.id];
}

const Cell &Cell_group::_get_cell(uint32_t index) const {
    return _cells[index].get();
}

bool Cell_group::contains(uint32_t cell_id) const {
    return find(cell_id) != Not_found;
}

Cell_group &Cell_group::operator = (const Cell_group &cg) {
    if (this != &cg){ //prevent self assigment object ruin (cg = cg)
        clear();
        for (uint32_t i = 0; i < cg.size() ; i++) add(cg[i]);
    }
    return *this;
}

Cell_group &Cell_group::operator += (const Cell_group &cg) {
    if (this != &cg) { //prevent self assigment object ruin (cg = cg)
        for (uint32_t i = 0; i < cg.size(); i++) add(cg[i]);
    }
    return *this;
}

bool Cell_group::remove(const Cell &cell) {
    if (find(cell.id) == Not_found) return false;
    Cell_group cg = *this;
    clear();
    for (uint32_t i = 0; i < cg.size(); i++) if (cg[i].id != cell.id) add(cg[i]);
    return true;
}

bool Cell_group::toggle(const Cell &cell) {
    return remove(cell)?true:add(cell);
}

bool Cell_group::add(const Cell &cell) {
    if (contains(cell.id)) return false;
    while(_id_index.size() <= cell.id) _id_index.push_back(Not_found);
    _id_index[cell.id] = _cells.size();
    _cells.emplace_back(cell);
    return true;
}

Cell_group::Cell_group() {
    clear();
}

bool Cell_group::contains(const Cell &cell) const {
    return contains(cell.id);
}

double Cell_group::distance(const Cell &c0, const Cell &c1) const {
    return sqrt(pow(c1.location.y-c0.location.y,2) + pow(c1.location.x-c0.location.x,2));
}

double Cell_group::distance(const uint32_t s, const uint32_t d) const {
    return distance(_cells[s].get(),_cells[d].get());
}

Cell_group Cell_group::operator+(const Cell_group &cg) {
    Cell_group ncg = *this;
    ncg += cg;
    return ncg;
}

Cell_group Cell_group::operator-(const Cell_group &cg) {
    Cell_group ncg = *this;
    for (uint32_t i=0; i<cg.size(); i++) ncg.remove(cg[i]);
    return ncg;
}

Cell_group Cell_group::operator-(const Cell &cell) {
    Cell_group ncg = *this;
    ncg.remove(cell);
    return ncg;
}

Cell_group &Cell_group::operator+=(const Cell &cell) {
    add(cell);
    return *this;
}

Cell_group &Cell_group::operator-=(const Cell &cell) {
    remove(cell);
    return *this;
}

Cell_group &Cell_group::operator-=(const Cell_group &cg) {
    for (uint32_t i=0 ; i<cg.size(); i++) remove(cg[i]);
    return *this;
}

Cell_group Cell_group::operator+(const Cell &cg) {
    Cell_group ncg=*this;
    ncg.add(cg);
    return Cell_group();
}

Cell_group Cell_group::random_shuffle() const {
    Cell_group cg;
    auto index = new_index(size());
    uint32_t seed = rand();
    std::shuffle(index.begin(),index.end(),std::default_random_engine(seed));
    for(uint32_t i:index) cg.add((*this)[i]);
    return cg;
}

Map::Map(const Cell_group &group)
:_group(group){
    for (auto & i : _coordinate_index) for (int & j : i) j = Not_found;
    for ( uint32_t i = 0; i < group.size() ; i++){
        int x = group[i].coordinates.x + 128;
        int y = group[i].coordinates.y + 128;
        _coordinate_index[x][y] = i;
    }
}

int32_t Map::find(const Coordinates &c) const {
    int x = c.x + 128;
    int y = c.y + 128;
    return _coordinate_index[x][y];
}

const Cell &Map::operator[](const Coordinates &c) const{
    return _group[find(c)];
}
