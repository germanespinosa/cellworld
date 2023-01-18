#pragma once
#include <cell_world/shape.h>
#include <cell_world/cell_group.h>
#include <cell_world/connection.h>
#include <cell_world/visibility.h>
#include <cell_world/graph.h>
#include <cell_world/paths.h>

namespace cell_world{

    struct World_info : json_cpp::Json_object {
        World_info() = default;
        World_info(const std::string &, const std::string &, const std::string &);
        std::string world_configuration;
        std::string world_implementation;
        std::string occlusions;
        Json_object_members(
                Add_member(world_configuration);
                Add_member(world_implementation);
                Add_member(occlusions);
        )
    };

    struct World_configuration : json_cpp::Json_object{
        World_configuration ();
        World_configuration (const World_configuration &);
        World_configuration (const Shape &, const Coordinates_list &, const Connection_pattern &);
        Shape cell_shape;
        Coordinates_list cell_coordinates;
        Connection_pattern connection_pattern;
        Json_object_members({
                                Add_member(cell_shape);
                                Add_member(connection_pattern);
                                Add_member(cell_coordinates);
                            })
        static World_configuration get_from_parameters_name(const std::string &);
    };

    struct World_implementation : json_cpp::Json_object{
        World_implementation ();
        World_implementation (const World_implementation &);
        World_implementation (const Location_list &, const Space &, const Transformation &);
        Location_list cell_locations;
        Space space;
        Transformation cell_transformation;
        void transform(const Space &);
        void scale( const Scale & scale);
        Json_object_members({
                                Add_member(cell_locations);
                                Add_member(space);
                                Add_member(cell_transformation);
                            })
        static World_implementation get_from_parameters_name(const std::string &, const std::string &);
    };

    struct World_statistics : json_cpp::Json_object{
        World_statistics() = default;

        Json_object_members(
                Add_member(spatial_entropy);
                Add_member(spatial_espinometry);
                Add_member(spatial_connections);
                Add_member(spatial_connections_derivative);
                Add_member(spatial_centrality);
                Add_member(spatial_centrality_derivative);
                Add_member(visual_entropy);
                Add_member(visual_espinometry);
                Add_member(visual_connections);
                Add_member(visual_connections_derivative);
                Add_member(visual_centrality);
                Add_member(visual_centrality_derivative);;
                Add_optional_member(ITOR_potential);
                Add_optional_member(ITOR_direction);
        )
        float spatial_entropy{};
        float spatial_espinometry{};
        json_cpp::Json_vector<unsigned int> spatial_connections{};
        json_cpp::Json_vector<unsigned int> spatial_connections_derivative{};
        json_cpp::Json_vector<float> spatial_centrality{};
        json_cpp::Json_vector<float> spatial_centrality_derivative{};
        float visual_entropy{};
        float visual_espinometry{};
        json_cpp::Json_vector<unsigned int> visual_connections{};
        json_cpp::Json_vector<unsigned int> visual_connections_derivative{};
        json_cpp::Json_vector<float> visual_centrality{};
        json_cpp::Json_vector<float> visual_centrality_derivative{};
        json_cpp::Json_vector<float> ITOR_potential{};
        json_cpp::Json_vector<Coordinates> ITOR_direction{};
        static World_statistics get_from_parameters_name(const std::string &, const std::string &);
    };

    struct World : json_cpp::Json_object{
        World ();
        World(const World_configuration &);
        World(const World_configuration &, const World_implementation &);
        World(const World_configuration &, const World_implementation &, const Cell_group_builder &);
        bool add(Cell);
        bool add(Coordinates);
        void set_occlusions(const Cell_group_builder &);
        unsigned int size() const;
        Cell &operator[](const unsigned int& );
        Cell_group create_cell_group() const;
        Cell_group create_cell_group(const Json_unsigned_int_vector &) const;
        Cell_group create_cell_group(const Cell_group_builder &) const;
        Cell_group create_cell_group(const std::string &) const;
        Graph create_graph() const;
        Graph create_graph(const Graph_builder &) const;
        Paths create_paths(const Path_builder &) const;
        Location_visibility create_location_visibility() const;
        World_configuration get_configuration();
        World_implementation get_implementation();
        Json_object_members({
                         Add_member(connection_pattern);
                         Add_member(cells);
                         Add_optional_member(cell_shape);
                         Add_optional_member(cell_transformation);
                         Add_optional_member(space);
                         Ignore_member("name");
        })
        Connection_pattern connection_pattern;
        Cell_list cells;
        Shape cell_shape;
        Transformation cell_transformation;
        Space space;
        World_statistics get_statistics(unsigned int depth = 1);
        static World get_from_parameters_name(const std::string &, const std::string &, const std::string &);
        static World get_from_parameters_name(const std::string &, const std::string &);
        static World get_from_world_info(const World_info &);
    };
}