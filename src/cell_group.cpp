#include <cell_world/cell_group.h>
#include <string>
#include <algorithm>
#include <random>
#include <cell_world/chance.h>
#include <cell_world/resources.h>

using namespace std;

namespace cell_world {

    const Cell &Cell_group::operator[](unsigned int index) const {
        return (begin() + index)->get();
    }

    void Cell_group::clear() {
        json_cpp::Json_vector<Cell_reference>::clear();
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

    bool Cell_group::add(const Cell &cell) {
        if (contains(cell.id)) return false;
        while (_id_index.size() <= cell.id) _id_index.push_back(Not_found);
        _id_index[cell.id] = size();
        emplace_back(cell);
        return true;
    }

    Cell_group::Cell_group() = default;

    bool Cell_group::contains(const Cell &cell) const {
        return contains(cell.id);
    }

     Cell_group Cell_group::operator+(const Cell_group &cg) const{
        Cell_group ncg = *this;
        ncg += cg;
        return ncg;
    }

    Cell_group Cell_group::operator-(const Cell_group &cg) const{
        Cell_group ncg;
        for (auto &c: *this) if (!cg.contains(c.get())) ncg.add(c.get());
        return ncg;
    }

    Cell_group Cell_group::operator-(const Cell &cell) const{
        Cell_group ncg = *this;
        for (auto &c: *this) if (c.get()!=cell) ncg.add(c.get());
        return ncg;
    }

    Cell_group &Cell_group::operator+=(const Cell &cell) {
        add(cell);
        return *this;
    }

    Cell_group Cell_group::operator+(const Cell &cg) const{
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
        for (auto &c:*this) if (c.get().occluded) cg.add(c.get());
        return cg;
    }

    Cell_group Cell_group::free_cells() const {
        Cell_group cg;
        for (auto &c:*this) if (!c.get().occluded) cg.add(c.get());
        return cg;
    }

    vector<float> Cell_group::get_distances(const Cell &bc) const {
        vector<float> d;
        for (auto &c:*this) d.push_back(bc.location.dist(c.get().location));
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
        return (*this)[Chance::dice(this->size())];
    }

    Cell_group Cell_group::operator&(const Cell_group &cg) const {
        Cell_group r;
        if (cg.size()<size()){
            for (auto &cr : cg){
                auto &c = cr.get();
                if (find(c.id)!=Not_found) r.add(c);
            }
        } else {
            for (auto &cr : *this){
                auto &c = cr.get();
                if (cg.find(c.id)!=Not_found) r.add(c);
            }
        }
        return r;
    }

    int Cell_group::find(const Location &location) const {
        if (empty()) return Not_found;
        float min_d = location.dist((*this)[0].location);
        int cell_id = 0;
        for(unsigned i = 1;i < size();i++){
            float d = location.dist((*this)[i].location);
            if (d < min_d) {
                min_d = d;
                cell_id = i;
            }
        }
        return cell_id;
    }

    int Cell_group::find(const Coordinates &coordinates) const {
        for(unsigned i = 1;i < size();i++){
            if (coordinates == (*this)[i].coordinates ){
                return i;
            }
        }
        return Not_found;
    }

    Cell_group_builder Cell_group::get_builder() const {
        Cell_group_builder builder;
        for (const Cell &cell:*this) builder.push_back(cell.id);
        return builder;
    }

    Cell_reference::Cell_reference(const Cell &cell):
            std::reference_wrapper<const Cell>(cell){ }

    void Cell_reference::json_write(ostream &o) const {
        o << this->get().id;
    }

    Cell_reference::operator const Cell &() const {
        return this->get();
    }

    Cell_group_builder Cell_group_builder::get_from_parameters_name(const std::string &configuration_name, const std::string &cell_group_name) {
        return Resources::from("cell_group").key(configuration_name).key(cell_group_name).get_resource<Cell_group_builder>();
    }

    Cell_group_builder Cell_group_builder::get_from_parameters_name(const string &configuration_name, const string &occlusions_name, const string &cell_group_name) {
        return Resources::from("cell_group").key(configuration_name).key(occlusions_name).key(cell_group_name).get_resource<Cell_group_builder>();
    }
}