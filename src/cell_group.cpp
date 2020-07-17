#include <cell_group.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <random>
#include <chance.h>

using namespace std;
using namespace ge211;

namespace cell_world {

    unsigned int Cell_group::size() const {
        return _cells.size();
    }

    const Cell &Cell_group::operator[](unsigned int index) const {
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

    bool Cell_group::save() const {
        bool res = save(_file_name);
        return res;
    }

    void Cell_group::clear() {
        _cells.clear();
        _id_index.clear();
    }

     int Cell_group::find(unsigned int cell_id) const {
        if (cell_id >= _id_index.size()) return Not_found;
        return _id_index[cell_id];
    }

     int Cell_group::find(const Cell &cell) const {
        if (cell.id >= _id_index.size()) return Not_found;
        return _id_index[cell.id];
    }

    const Cell &Cell_group::_get_cell(unsigned int index) const {
        return _cells[index].get();
    }

    bool Cell_group::contains(unsigned int cell_id) const {
        return find(cell_id) != Not_found;
    }

    Cell_group &Cell_group::operator=(const Cell_group &cg) {
        if (this != &cg) { //prevent self assigment object ruin (cg = cg)
            clear();
            for (unsigned int i = 0; i < cg.size(); i++) add(cg[i]);
        }
        return *this;
    }

    Cell_group &Cell_group::operator+=(const Cell_group &cg) {
        if (this != &cg) { //prevent self assigment object ruin (cg = cg)
            for (unsigned int i = 0; i < cg.size(); i++) add(cg[i]);
        }
        return *this;
    }

    bool Cell_group::remove(const Cell &cell) {
        if (find(cell.id) == Not_found) return false;
        Cell_group cg = *this;
        clear();
        for (unsigned int i = 0; i < cg.size(); i++) if (cg[i].id != cell.id) add(cg[i]);
        return true;
    }

    bool Cell_group::toggle(const Cell &cell) {
        return remove(cell) ? true : add(cell);
    }

    bool Cell_group::add(const Cell &cell) {
        if (contains(cell.id)) return false;
        while (_id_index.size() <= cell.id) _id_index.push_back(Not_found);
        _id_index[cell.id] = _cells.size();
        _cells.emplace_back(cell);
        return true;
    }

    Cell_group::Cell_group() = default;

    bool Cell_group::contains(const Cell &cell) const {
        return contains(cell.id);
    }

    double Cell_group::distance(const Cell &c0, const Cell &c1) {
        return sqrt(pow(c1.location.y - c0.location.y, 2) + pow(c1.location.x - c0.location.x, 2));
    }

    double Cell_group::distance(const unsigned int s, const unsigned int d) const {
        return distance(_cells[s].get(), _cells[d].get());
    }

    Cell_group Cell_group::operator+(const Cell_group &cg) {
        Cell_group ncg = *this;
        ncg += cg;
        return ncg;
    }

    Cell_group Cell_group::operator-(const Cell_group &cg) {
        Cell_group ncg = *this;
        for (unsigned int i = 0; i < cg.size(); i++) ncg.remove(cg[i]);
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
        for (unsigned int i = 0; i < cg.size(); i++) remove(cg[i]);
        return *this;
    }

    Cell_group Cell_group::operator+(const Cell &cg) {
        Cell_group ncg = *this;
        ncg.add(cg);
        return Cell_group();
    }

    Cell_group Cell_group::random_shuffle() const {
        Cell_group cg;
        auto index = new_index(size());
        unsigned int seed = Chance::dice();
        std::shuffle(index.begin(), index.end(), std::default_random_engine(seed));
        for (unsigned int i:index) cg.add((*this)[i]);
        return cg;
    }

    Cell_group Cell_group::occluded_cells() const {
        Cell_group cg;
        for (auto &c:_cells) if (c.get().occluded) cg.add(c.get());
        return cg;
    }

    Cell_group Cell_group::free_cells() const {
        Cell_group cg;
        for (auto &c:_cells) if (!c.get().occluded) cg.add(c.get());
        return cg;
    }

    std::ostream &operator<<(std::ostream &out, const Cell_group &cg) {
        out << "[" ;
        for (unsigned int i=0;i<cg.size();i++) {
            out << (i?", ":"") << cg[i].id;
        }
        out << "]";
        return out;
    }

    vector<double> Cell_group::get_distances(const Cell &bc) const {
        vector<double> d;
        for (auto &c:_cells) d.push_back(bc.location.dist(c.get().location));
        return d;
    }

    bool Cell_group::operator==(const Cell_group &cg) const {
        if (size()!=cg.size()) return false;
        for (unsigned int i=0;i<cg.size();i++) if (!contains(cg[i])) return false;
        return true;
    }

    bool Cell_group::operator!=(const Cell_group &cg) const {
        return !(*this == cg);
    }

    const Cell &Cell_group::random_cell() const {
        return _cells[Chance::dice(_cells.size())].get();
    }

    Map::Map(const Cell_group &group)
            : _group(group) {
        _coordinate_index = vector<vector< int>>(256,vector< int>(256,Not_found));
        for (unsigned int i = 0; i < group.size(); i++) {
            auto x = (uint8_t) group[i].coordinates.x;
            auto y = (uint8_t) group[i].coordinates.y;
            _coordinate_index[x][y] = i;
        }
    }

     int Map::find(const Coordinates &c) const {
        auto x = (uint8_t) c.x;
        auto y = (uint8_t) c.y;
        return _coordinate_index[x][y];
    }

    const Cell &Map::operator[](const Coordinates &c) const {
        return _group[find(c)];
    }
}