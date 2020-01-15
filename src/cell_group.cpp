#include <cell_group.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;
using namespace cell_world;
using namespace ge211;

uint32_t Cell_group::size() const {
    return _cells.size();
}

const Cell &Cell_group::operator[](uint32_t index) const {
    return _get_cell(index);
}

bool Cell_group::save(const std::string &file_path) const {
    std::ofstream file;
    file.open(file_path.c_str());
    for (auto _cell : _cells) {
        file << _cell->id;
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
    for (auto & i : _coordinate_index) for (int & j : i) j = Not_found;
}

int32_t Cell_group::find(Coordinates coordinates) const {
    uint32_t i = ( coordinates.x + 128 );
    uint32_t j = ( coordinates.y + 128 );
    return _coordinate_index[i][j];
}

int32_t Cell_group::find(uint32_t cell_id) const {
    if (cell_id>=_id_index.size()) return Not_found;
    return _id_index[cell_id];
}

const Cell &Cell_group::_get_cell(uint32_t index) const {
    return *_cells[index];
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
    _cells.push_back(&cell);
    uint32_t i = ( cell.coordinates.x + 128 );
    uint32_t j = ( cell.coordinates.y + 128 );
    _coordinate_index[i][j] = cell.id;
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
    return distance(*_cells[s],*_cells[d]);
}
