#include <world.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <utility>
#include <any>
using namespace std;
using namespace ge211;

namespace cell_world {
    bool World::add(Cell &cell) {
        cell.id = _cells.size();
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
            int ct, cx, cy;
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
                    << cell.cell_type << " "
                    << cell.coordinates.x << " "
                    << cell.coordinates.y << " "
                    << cell.location.x << " "
                    << cell.location.y << " "
                    << cell.value << " "
                    << cell.occluded;
            file << std::endl;
        }
        return true;
    }

    unsigned int World::size() const {
        return _cells.size();
    }

    Cell &World::operator[](const unsigned int &id) {
        return _cells[id];
    }

    void World::set_occlusion(unsigned int id, bool occluded) {
        _cells[id].occluded = occluded;
    }

    void World::set_value(unsigned int id, double value) {
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

    void World::json_set_parser(Json_parser &p) {
        p.json_add_member("name",true,name);
        p.json_add_member("connection_pattern",true, connection_pattern);
        p.json_add_member("cells",true, _cells);
    }

    World::World() : World(""){
    }

    Cell_group World::create_cell_group() const {
        Cell_group cg;
        for (const auto & _cell : _cells) cg.add(_cell);
        return cg;
    }

    Cell_group World::create_cell_group(const std::vector<unsigned int> &cell_ids) const {
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
             int cell_id;
            Cell cell;
            ss >> cell_id;
            cg.add(_cells[cell_id]);
        }
        return cg;
    }

    void World::set_direction(unsigned int index, const Coordinates &direction) {
        _cells[index].direction = direction;
    }

    Graph World::create_graph() const {
        return connection_pattern.get_graph(create_cell_group());
    }

    Paths World::create_paths(Paths::Path_type type) const {
        Graph g = create_graph();
        Paths p( g, type);
        p._name = name;
        return p;
    }

    Paths World::create_paths(const std::string paths_name, Paths::Path_type type) const {
        Paths p;
        Graph g = create_graph();
        p.type = type;
        p._name=paths_name;
        p._cells = g.nodes;
        string file_path = paths_name + "_" + Paths::_type_string(type) +  p._extension;
        std::ifstream file;
        file.open(file_path.c_str());
        if (!file.good()) return p;
        string line;
        while (getline(file, line)) {
            Connection_pattern cp;
            cp.load_from_string(line);
            p._next_move.push_back(cp);
        }
        return p;
    }
}