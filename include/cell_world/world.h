#pragma once
#include <cell_world/core.h>
#include <cell_world/shape.h>
#include <cell_world/cell_group.h>
#include <cell_world/connection.h>
#include <cell_world/visibility.h>
#include <cell_world/graph.h>
#include <cell_world/paths.h>

namespace cell_world{

    struct World_info : json_cpp::Json_object {
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
        World_configuration (const Shape &, const Coordinates_list &, const Connection_pattern &);
        Shape cell_shape;
        Coordinates_list cell_coordinates;
        Connection_pattern connection_pattern;
        Json_object_members({
                                Add_member(cell_shape);
                                Add_member(connection_pattern);
                                Add_member(cell_coordinates);
                            })
    };

    struct World_implementation : json_cpp::Json_object{
        World_implementation ();
        World_implementation (const Location_list &, const Space &, const Transformation &);
        Location_list cell_locations;
        Space space;
        Transformation cell_transformation;
        void transform(const Space &);
        Json_object_members({
                                Add_member(cell_locations);
                                Add_member(space);
                                Add_member(cell_transformation);
                            })
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
        Cell_group create_cell_group(const Cell_group_builder &) const;
        Cell_group create_cell_group(const std::string&) const;
        Graph create_graph() const;
        Graph create_graph(const Graph_builder &) const;
        Paths create_paths(const Path_builder &) const;
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
    };
}