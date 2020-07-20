#include <cell_world/world.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <utility>
#include <any>
using namespace std;

namespace cell_world {
    bool World::add(Cell &cell) {
        cell.id = cells.size();
        cells.emplace_back(cell);
        return true;
    }

    unsigned int World::size() const {
        return cells.size();
    }

    Cell &World::operator[](const unsigned int &id) {
        return cells[id];
    }

    void World::set_occlusion(unsigned int id, bool occluded) {
        cells[id].occluded = occluded;
    }

    void World::set_value(unsigned int id, double value) {
        cells[id].value = value;
    }

    World::World(std::string name) : name(std::move(name)) {
    }

    World::World() : World(""){
    }

    Cell_group World::create_cell_group() const {
        Cell_group cg;
        for (const auto & _cell : cells) cg.add(_cell);
        return cg;
    }

    Cell_group World::create_cell_group(const std::vector<unsigned int> &cell_ids) const {
        Cell_group cg;
        for (auto id : cell_ids) cg.add(cells[id]);
        return cg;
    }

    Cell_group World::create_cell_group(const std::string &file_path) const {
        Cell_group cg;
        json_cpp::Json_vector<unsigned int> ids;
        ids.load(file_path);
        for (auto id:ids) cg.add(cells[id]);
        return cg;
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
        Graph g = create_graph();
        Paths p;
        p._cells = g.nodes;
        string file_path = paths_name + "_" + Paths::_type_string(type) +  p._extension;
        p.load(file_path);
        return p;
    }
}