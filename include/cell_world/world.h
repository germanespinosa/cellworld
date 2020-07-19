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
        void set_direction(unsigned int, const Coordinates &);
        Cell_group create_cell_group() const;
        Cell_group create_cell_group(const std::vector<unsigned int>&) const;
        Cell_group create_cell_group(const std::string&) const;
        Paths create_paths(Paths::Path_type) const;
        Paths create_paths(const std::string, Paths::Path_type) const;
        Graph create_graph() const;
        Json_set_builder({
            Json_add_member(name,true);
            Json_add_member(connection_pattern,true);
            Json_add_member(cells,true);
        })
        std::string name;
        Connection_pattern connection_pattern;
        Cell_list cells;
    };
}