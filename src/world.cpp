#include <world.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <utility>

using namespace std;

using namespace ge211;
namespace cell_world {
    bool World::add(Cell cell) {
        cell.id = _cells.size();
        vector<double> distances;
        _cells.emplace_back(cell);
        return true;
    }

    bool World::load(const std::string &world_name) {
        string file_path = world_name + _extension;
        _cells.clear();
        std::ifstream file;
        file.open(file_path.c_str());
        string line;
        if (getline(file, line)) connection_pattern.load_from_string(line);
        while (getline(file, line)) {
            istringstream ss(line);
            int16_t ct, cx, cy;
            Cell cell;
            ss >> ct;
            ss >> cx;
            ss >> cy;
            ss >> cell.location.x;
            ss >> cell.location.y;
            ss >> cell.value;
            ss >> cell.occluded;
            cell.cell_type = (Cell_type) ct;
            cell.coordinates.x = cx;
            cell.coordinates.y = cy;
            if (!add(cell)) return false;
        }
        return true;
    }

    bool World::save(const std::string &world_name) const {
        string file_path = world_name + _extension;
        std::ofstream file;
        file.open(file_path.c_str());
        if (!file.good()) return false;
        file << connection_pattern.save_to_string() << endl;
        for (const auto &cell : _cells) {
            file
                    << (int16_t) cell.cell_type << " "
                    << (int16_t) cell.coordinates.x << " "
                    << (int16_t) cell.coordinates.y << " "
                    << cell.location.x << " "
                    << cell.location.y << " "
                    << cell.value << " "
                    << cell.occluded;
            file << std::endl;
        }
        return true;
    }

    uint32_t World::size() const {
        return _cells.size();
    }

    Cell &World::operator[](const uint32_t &id) {
        return _cells[id];
    }

    void World::set_occlusion(uint32_t id, bool occluded) {
        _cells[id].occluded = occluded;
    }

    void World::set_value(uint32_t id, double value) {
        _cells[id].value = value;
    }

    bool World::save() const {
        return save(name);
    }

    bool World::load() {
        return load(name);
    }

    World::World(std::string name) : name(std::move(name)) {
    }

    Cell_group World::create_cell_group() const {
        Cell_group cg;
        for (const auto & _cell : _cells) cg.add(_cell);
        return cg;
    }

    Cell_group World::create_cell_group(const std::vector<uint32_t> &cell_ids) const {
        Cell_group cg;
        for (auto id : cell_ids) cg.add(_cells[id]);
        return cg;
    }

    Cell_group World::create_cell_group(const std::string &group_name) const {
        Cell_group cg;
        string file_path = group_name + cg._extension;
        std::ifstream file;
        file.open(file_path.c_str());
        if (!file.good()) return cg;
        string line;
        while (getline(file, line)) {
            istringstream ss(line);
            int32_t cell_id;
            Cell cell;
            ss >> cell_id;
            cg.add(_cells[cell_id]);
        }
        return cg;
    }

    void World::set_direction(uint32_t index, const Coordinates &direction) {
        _cells[index].direction = direction;
    }

    Graph World::create_graph() const {
        return connection_pattern.get_graph(create_cell_group());
    }

    std::ostream &operator<<(std::ostream &out, const World &world) {
        for (const auto & _cell : world._cells) out << _cell << endl;
        return out;
    }
}