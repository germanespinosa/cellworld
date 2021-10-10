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

    World::World(std::string name) : name(std::move(name)) {
    }

    World::World() : World(""){
    }

    Cell_group World::create_cell_group() const {
        Cell_group cg;
        for (const auto & _cell : cells) cg.add(_cell);
        return cg;
    }

    Cell_group World::create_cell_group(const Cell_group_builder &cell_ids) const {
        Cell_group cg;
        for (auto id : cell_ids) cg.add(cells[id]);
        return cg;
    }

    Cell_group World::create_cell_group(const std::string &file_path) const {
        Cell_group cg;
        Cell_group_builder ids;
        ids.load(file_path);
        return create_cell_group(ids);
    }

    Graph World::create_graph() const {
        return connection_pattern.get_graph(create_cell_group());
    }

    Paths World::create_paths(const Path_builder &pb) const {
        Graph graph = create_graph();
        return Paths(graph, pb);
    }

    Graph World::create_graph(const Graph_builder &gb) const {
        Graph g(create_cell_group());
        for (unsigned int ids=0;ids<gb.size();ids++){
            auto &sc = g[ids];
            for (unsigned int idd:gb[ids]){
                auto &d=g.cells[idd];
                sc.add(d);
            }
        }
        return g;
    }

    World::World(
            std::string name,
            const World_configuration &world_configuration,
            const Location_list &cell_locations):
            name(name),
            connection_pattern(world_configuration.connection_pattern),
            cell_descriptor(world_configuration.cell_descriptor)
    {
        unsigned int id = 0;
        for (auto &coord:world_configuration.cell_coordinates){
            Cell cell;
            cell.id = id;
            cell.coordinates = coord;
            cell.location = cell_locations[id];
            id++;
        }
    }

    World::World(std::string name,
                 const World_configuration & world_configuration,
                 const Location_list &cell_locations,
                 const Cell_group_builder &occlusions):
                 World(name, world_configuration, cell_locations){
        for (auto &occlusion: occlusions){
            cells[occlusion].occluded = true;
        }
    }
}