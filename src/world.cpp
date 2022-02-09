#include <cell_world/world.h>
#include <string>

using namespace std;

namespace cell_world {
    World_configuration::World_configuration() = default;


    World_configuration::World_configuration(const Shape &cell_shape,
                                             const Coordinates_list &cell_coordinates,
                                             const Connection_pattern & connection_pattern):
            cell_shape(cell_shape),
            cell_coordinates(cell_coordinates),
            connection_pattern(connection_pattern){

    }

    World_implementation::World_implementation() = default;


    World_implementation::World_implementation(const Location_list &cell_locations,
                                                           const Space &space,
                                                           const Transformation &cell_transformation):
                                                           cell_locations(cell_locations),
                                                           space(space),
                                                           cell_transformation(cell_transformation){

    }

    void World_implementation::transform(const Space &s) {
        auto size_ratio = s.transformation.size / space.transformation.size;
        auto rotation_dif = s.transformation.rotation - space.transformation.rotation;
        cell_transformation.size =  cell_transformation.size * size_ratio;
        cell_transformation.rotation = cell_transformation.rotation + rotation_dif;
        for (auto &cell_location : cell_locations){
            auto t = space.center.transformation(cell_location);
            t.size *= size_ratio;
            t.rotation += rotation_dif;
            cell_location = s.center;
            cell_location.transform(t);
        }
        space = s;
    }

// World

    World::World() = default;


    bool World::add(Cell cell) {
        cell.id = cells.size();
        cells.push_back(cell);
        return true;
    }

    unsigned int World::size() const {
        return cells.size();
    }

    Cell &World::operator[](const unsigned int &id) {
        return cells[id];
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


    void World::set_occlusions(const Cell_group_builder &occlusions) {
        for (auto &cell: cells){
            cell.occluded = false;
        }
        for (auto &occlusion: occlusions){
            cells[occlusion].occluded = true;
        }
    }

    World::World(const World_configuration &world_configuration):
    connection_pattern(world_configuration.connection_pattern),
    cell_shape(world_configuration.cell_shape){
        for(auto &c:world_configuration.cell_coordinates) add(c);
    }

    World::World(const World_configuration &world_configuration, const World_implementation &world_implementation):
    World(world_configuration){
        for (Cell &cell: cells)
            cell.location = world_implementation.cell_locations[cell.id];
        cell_transformation = world_implementation.cell_transformation;
        space = world_implementation.space;
    }

    bool World::add(Coordinates coordinates) {
        return add(Cell(coordinates));
    }

    World::World(const World_configuration &world_configuration, const World_implementation &world_implementation, const Cell_group_builder &occlusions) :
        World(world_configuration,world_implementation)
    {
        set_occlusions(occlusions);
    }

    World_implementation World::get_implementation() {
        World_implementation implementation;
        for (auto &cell : cells) {
            implementation.cell_locations.push_back(cell.location);
        }
        implementation.space = space;
        implementation.cell_transformation = cell_transformation;
        return implementation;
    }

    World_configuration World::get_configuration() {
        World_configuration configuration;
        for (auto &cell : cells) {
            configuration.cell_coordinates.push_back(cell.coordinates);
        }
        configuration.cell_shape = cell_shape;
        configuration.connection_pattern = connection_pattern;
        return configuration;
    }
}


