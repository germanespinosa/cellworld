#include <cell_world/core.h>
#include <cell_world/world.h>
#include <cell_world/resources.h>
#include <cell_world/visibility.h>
#include <cell_world/map.h>
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

    World_configuration::World_configuration(const World_configuration &wc):
    cell_shape(wc.cell_shape),
    cell_coordinates(wc.cell_coordinates),
    connection_pattern(wc.connection_pattern){
    }

    World_configuration World_configuration::get_from_parameters_name(const string &configuration_name) {
        return Resources::from("world_configuration").key(configuration_name).get_resource<World_configuration>();
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

    void World_implementation::scale(const Scale &scale) {
        for (auto &location : cell_locations){
            location  = space.scale(location, scale);
        }
    }

    World_implementation::World_implementation(const World_implementation &wi):
            cell_locations(wi.cell_locations),
            space(wi.space),
            cell_transformation(wi.cell_transformation){
    }

    World_implementation World_implementation::get_from_parameters_name(const string &configuration_name, const string &implementation_name) {
        return Resources::from("world_implementation").key(configuration_name).key(implementation_name).get_resource<World_implementation>();
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
        return create_cell_group((Json_unsigned_int_vector &) cell_ids);
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

    Location_visibility World::create_location_visibility() const {
        return {create_cell_group(), cell_shape, cell_transformation};
    }

    World World::get_from_parameters_name(const std::string &configuration_name, const std::string &implementation_name, const std::string &occlusions_name) {
        auto world = get_from_parameters_name(configuration_name, implementation_name);
        auto occlusions = Cell_group_builder::get_from_parameters_name(configuration_name, occlusions_name + ".occlusions");
        world.set_occlusions(occlusions);
        return world;
    }

    World World::get_from_parameters_name(const std::string &configuration_name, const std::string &implementation_name) {
        auto configuration = World_configuration::get_from_parameters_name(configuration_name);
        auto implementation = World_implementation::get_from_parameters_name(configuration_name, implementation_name);
        return {configuration, implementation};
    }

    World World::get_from_world_info(const World_info &world_info) {
        auto world = get_from_parameters_name(world_info.world_configuration, world_info.world_implementation);
        auto occlusions = Cell_group_builder::get_from_parameters_name(world_info.world_configuration, world_info.occlusions + ".occlusions");
        world.set_occlusions(occlusions);
        return world;
    }

    World_statistics World::get_statistics(unsigned int depth) {
        World_statistics stats;
        auto cg = this->create_cell_group();
        auto map = Map(cg);
        auto spatial = this->create_graph();
        auto visual = Coordinates_visibility::create_graph(cg,this->cell_shape, this->cell_transformation);


        stats.spatial_connections = json_cpp::Json_vector<unsigned int>(cg.size(), 0);
        stats.spatial_centrality = spatial.get_centrality(depth);

        stats.visual_connections = json_cpp::Json_vector<unsigned int>(cg.size(), 0);
        stats.visual_centrality = visual.get_centrality(depth);

        for (const Cell &cell:cg){
            stats.spatial_connections[cell.id] = spatial[cell].size();
            stats.visual_connections[cell.id] = visual[cell].size();
        }

        stats.spatial_connections_derivative = json_cpp::Json_vector<unsigned int>(cg.size(), 0);
        stats.spatial_centrality_derivative = json_cpp::Json_vector<float>(cg.size(), 0);

        stats.visual_connections_derivative = json_cpp::Json_vector<unsigned int>(cg.size(), 0);
        stats.visual_centrality_derivative = json_cpp::Json_vector<float>(cg.size(), 0);

        stats.ITOR_direction = json_cpp::Json_vector<Coordinates>(cg.size());
        stats.ITOR_potential = json_cpp::Json_vector<float>(cg.size(), 0);

        auto pairs = this->connection_pattern.get_pairs();

        for (const Cell &cell:cells){
            if (cell.occluded) continue;
            stats.spatial_connections_derivative[cell.id] = 1;
            stats.spatial_centrality_derivative[cell.id] = 1;
            stats.visual_connections_derivative[cell.id] = 1;
            stats.visual_centrality_derivative[cell.id] = 1;

            auto cell_coordinates = cell.coordinates;
            int min_shared_visibility = -1;
            stats.ITOR_direction[cell.id] = Coordinates(0,0);
            for (auto &pair : pairs){
                auto first_coordinates = cell_coordinates + pair[0];
                auto first_cell_index = map.find(first_coordinates);

                auto second_coordinates = cell_coordinates + pair[1];
                auto second_cell_index = map.find(second_coordinates);

                float first_spatial_connection = 0;
                float first_spatial_centrality = 0;

                float first_visual_connection = 0;
                float first_visual_centrality = 0;

                if (first_cell_index!=Not_found) {
                    auto first_cell_id = cells[first_cell_index].id;
                    if (!cells[cell.id].occluded && !cells[first_cell_id].occluded) {
                        int shared_visibility = (visual[first_cell_id] & visual[cell.id]).size();
                        if (min_shared_visibility == -1 || shared_visibility < min_shared_visibility) {
                            min_shared_visibility = shared_visibility;
                            stats.ITOR_direction[cell.id] = cells[first_cell_id].coordinates;
                        }
                    }

                    first_spatial_connection = stats.spatial_connections[first_cell_id];
                    first_spatial_centrality = stats.spatial_centrality[first_cell_id];

                    first_visual_connection = stats.visual_connections[first_cell_id];
                    first_visual_centrality = stats.visual_centrality[first_cell_id];
                }

                float second_spatial_connection = 0;
                float second_spatial_centrality = 0;

                float second_visual_connection = 0;
                float second_visual_centrality = 0;

                if (second_cell_index!=Not_found) {
                    auto second_cell_id = cells[second_cell_index].id;
                    if (!cells[cell.id].occluded && !cells[second_cell_id].occluded) {
                        int shared_visibility = (visual[second_cell_id] & visual[cell.id]).size();
                        if (min_shared_visibility == -1 || shared_visibility < min_shared_visibility) {
                            min_shared_visibility = shared_visibility;
                            stats.ITOR_direction[cell.id] = cells[second_cell_id].coordinates;
                        }
                    }

                    second_spatial_connection = stats.spatial_connections[second_cell_id];
                    second_spatial_centrality = stats.spatial_centrality[second_cell_id];

                    second_visual_connection = stats.visual_connections[second_cell_id];
                    second_visual_centrality = stats.visual_centrality[second_cell_id];
                }
                stats.spatial_connections_derivative[cell.id] *= abs(first_spatial_connection-second_spatial_connection);
                stats.spatial_centrality_derivative[cell.id] *= abs(first_spatial_centrality-second_spatial_centrality);

                stats.visual_connections_derivative[cell.id] *= abs(first_visual_connection-second_visual_connection);
                stats.visual_centrality_derivative[cell.id] *= abs(first_visual_centrality-second_visual_centrality);
            }
            if(cells[cell.id].occluded) {
                stats.ITOR_potential[cell.id] = 0;
            } else {
                stats.ITOR_potential[cell.id] =
                        ((float) stats.visual_connections[cell.id] - (float) min_shared_visibility) /
                        (float) stats.visual_connections[cell.id];
            }
        }
        stats.spatial_entropy = labels_entropy(stats.spatial_connections);
        stats.visual_entropy = labels_entropy(stats.visual_connections);
        stats.spatial_espinometry = labels_entropy(stats.spatial_connections_derivative);
        stats.visual_espinometry = labels_entropy(stats.visual_connections_derivative);
        return stats;
    }

    World_statistics World_statistics::get_from_parameters_name(const string &configuration, const string &occlusions) {
        return Resources::from("world_statistics").key(configuration).key(occlusions).get_resource<World_statistics>();
    }

    World_info::World_info(const std::string &configuration_name, const std::string &implementation_name, const std::string &occlusions_name):
    world_configuration(configuration_name),
    world_implementation(implementation_name),
    occlusions(occlusions_name){

    }

    Cell_group World::create_cell_group(const Json_unsigned_int_vector &cell_ids) const {
        Cell_group cg;
        for (auto id : cell_ids) cg.add(cells[id]);
        return cg;
    }
}


