#pragma once
#include <cell_world/core.h>
#include <cell_world/cell_group.h>

namespace cell_world{
    struct Map{
        explicit Map(const Cell_group &);
        const Cell& operator [](const Coordinates &) const;
        const Cell& operator [](const Location &) const;
        int find (const Coordinates &) const;
        int find (const Location &) const;
        const Cell_group cells;
        Coordinates_list coordinates;
    private:
        std::vector< int> _coordinate_index;
        int base_y, base_x, columns, rows;
        int _index(const Coordinates &) const;
        int _index(const Location &) const;
    };
}