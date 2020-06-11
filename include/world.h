#pragma once
#include <core.h>
#include <cell_group.h>
#include <connection.h>
#include <visibility.h>
#include <graph.h>
#include<paths.h>
namespace cell_world{
    struct World{
        World() = default;
        explicit World(std::string );
        bool add(Cell&);
        bool load(const std::string&);
        bool load();
        bool save() const;
        bool save(const std::string&) const;
        unsigned int size() const;
        Cell &operator[](const unsigned int& );
        void set_occlusion(unsigned int, bool);
        void set_value(unsigned int, double);
        void set_direction(unsigned int, const Coordinates &);
        Cell_group create_cell_group() const;
        Cell_group create_cell_group(const std::vector<unsigned int>&) const;
        Cell_group create_cell_group(const std::string&) const;
        Paths create_paths(Paths::Path_type) const;
        Paths create_paths(std::string, Paths::Path_type) const;
        Graph create_graph() const;
        std::string name;
        Connection_pattern connection_pattern;
        friend std::ostream& operator<<(std::ostream& , const World& );
        friend std::istream & operator >> (std::istream &, World&);
    private:
        std::vector<Cell> _cells;
        const std::string _extension = ".world";
    };
}