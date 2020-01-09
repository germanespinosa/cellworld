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
    _index.clear();
}

int32_t Cell_group::find(Coordinates coordinates) const {
    uint32_t i = ( coordinates.x + 128 );
    uint32_t j = ( coordinates.y + 128 );
    return _map[i][j];
}

int32_t Cell_group::find(uint32_t cell_id) const {
    if (cell_id>=_index.size()) return Not_found;
    return _index[cell_id];
}

void Cell_group::get_connections(Connections &connections, const std::vector<Coordinates>& pattern) const {
    connections.clear();
    for (unsigned int source = 0; source < _cells.size(); source++){
        if ( !_cells[source]->occluded ) {
            for (auto j : pattern) {
                Coordinates c = _cells[source]->coordinates + j;
                int32_t destination = this->find(c);
                if (destination != Not_found && !_cells[destination]->occluded) {
                    connections.add(source,destination);
                }
            }
        }
    }
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
        for (uint32_t i = 0; i < cg.size() ; i++) _cells.push_back(&cg[i]);
    }
    return *this;
}

Cell_group &Cell_group::operator += (const Cell_group &cg) {
    for (uint32_t i = 0; i < cg.size() ; i++) add(cg[i]);
    return *this;
}

bool Cell_group::remove(const Cell &cell) {
    if (find(cell.id) == Not_found) return false;
    Cell_group cg = *this;
    this->clear();
    for (uint32_t i = 0; i < cg.size(); i++) if (cg[i].id != cell.id) this->add(cg[i]);
    return true;
}

bool Cell_group::toggle(const Cell &cell) {
    return remove(cell)?true:add(cell);
}

bool Cell_group::add(const Cell &cell) {
    if (contains(cell.id)) return false;
    while(_index.size() <= cell.id) _index.push_back(Not_found);
    _index[cell.id] = _cells.size();
    _cells.push_back(&cell);
    uint32_t i = ( cell.coordinates.x + 128 );
    uint32_t j = ( cell.coordinates.y + 128 );
    _map[i][j] = cell.id;
    return true;
}

Cell_group::Cell_group() {
    for (auto & i : _map) for (int & j : i) j = Not_found;
}
