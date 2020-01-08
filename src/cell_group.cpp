#include <cell_group.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;
using namespace cellworld;
using namespace ge211;

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

        while(_index.size()<=cell_id) _index.push_back(Not_found);

        _index[cell_id]=_cell_ids.size();
        _cell_ids.push_back(cell_id);
        return true;
    }
    return false;
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
    _index.clear();
}

int32_t Cell_group::find(Coordinates coordinates) const {
    int32_t  index = _world.find(coordinates);
    if (index == Not_found ) return Not_found;
    return find(index);
}

int32_t Cell_group::find(uint32_t cell_id) const {
    if (cell_id>=_index.size()) return Not_found;
    return _index[cell_id];
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

bool Cell_group::contains(uint32_t cell_id) const {
    return find(cell_id) != Not_found;
}

Cell_group &Cell_group::operator = (Cell_group &cg) {
    clear();
    for (uint32_t i = 0; i < cg.size() ; i++) _cell_ids.push_back(cg[i].id);
    return cg;
}

Cell_group &Cell_group::operator += (Cell_group &cg) {
    for (uint32_t i = 0; i < cg.size() ; i++) add(cg[i].id);
    return cg;
}

bool Cell_group::remove(uint32_t cell_id) {
    if (find(cell_id) == Not_found) return false;
    Cell_group cg = *this;
    this->clear();
    for (uint32_t i = 0; i < cg.size(); i++) if (cg[i].id != cell_id) this->add(cg[i].id);
    return true;
}

bool Cell_group::toggle(uint32_t cell_id) {
    return remove(cell_id)?true:add(cell_id);
}
