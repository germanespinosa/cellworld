#pragma once
#include <core.h>
#include <cell_group.h>
#include <connection.h>
#include <visibility.h>

namespace cell_world{

    struct World{
        explicit World(std::string );
        bool add(Cell);
        bool load(const std::string&);
        bool load();
        bool save() const;
        bool save(const std::string&) const;
        uint32_t size() const;
        Cell &operator[](const uint32_t& );
        void set_occlusion(uint32_t, bool);
        void set_value(uint32_t, double);
        void set_direction(uint32_t, const Coordinates &);
        Cell_group create_cell_group() const;
        Cell_group create_cell_group(const std::vector<uint32_t>&) const;
        Cell_group create_cell_group(const std::string&) const;
        std::string name;
        Connection_pattern connection_pattern;
        Cell_type cell_type;
    private:
        std::vector<Cell> _cells;
        const std::string _extension = ".world";
    };
}