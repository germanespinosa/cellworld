#pragma once
#include <cell_world/core.h>
#include <cell_world/cell_group.h>
#include <cell_world/connection.h>
#include <cell_world/visibility.h>
#include <cell_world/graph.h>
#include<cell_world/paths.h>
namespace cell_world{
    struct World : json_cpp::Json_object{
        World();
        explicit World(std::string );
        bool add(Cell&);
        unsigned int size() const;
        Cell &operator[](const unsigned int& );
        void set_occlusion(unsigned int, bool);
        void set_value(unsigned int, double);
        Cell_group create_cell_group() const;
        Cell_group create_cell_group(const Cell_group_builder &) const;
        Cell_group create_cell_group(const std::string&) const;
        Graph create_graph() const;
        Paths create_paths(const Path_builder &) const;
        Json_object_members({
                         Add_member(name);
                         Add_member(connection_pattern);
                         Add_member(cells);
        })
        std::string name;
        Connection_pattern connection_pattern;
        Cell_list cells;
    };
}