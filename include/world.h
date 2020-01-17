#pragma once
#include <core.h>
#include <ge211.h>
#include "cell_group.h"
#include "connection.h"


namespace cell_world{

    struct World{
        explicit World(std::string );
        bool add(Cell);
        bool load(const std::string&);
        bool load();
        bool save() const;
        bool save(const std::string&) const;
        double distance(uint32_t, uint32_t) const;
        double distance(const Cell&, const Cell&) const;
        double distance(const Cell&, const Cell&, const Cell&) const;
        uint32_t size() const;
        const Cell &operator[](const uint32_t& ) const;
        void set_occlusion(uint32_t, bool);
        void set_value(uint32_t, double);
        Cell_group create_cell_group() const;
        Cell_group create_cell_group(const std::vector<uint32_t>&) const;
        Cell_group create_cell_group(std::string) const;
        std::string name;
    private:
        std::vector<Cell> _cells;
        std::vector<std::vector<double>> _distances;
        const std::string _extension = ".world";
    };
}