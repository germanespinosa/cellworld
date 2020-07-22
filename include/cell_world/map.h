#pragma once
#include <cell_world/core.h>
#include <cell_world/cell_group.h>

namespace cell_world{
    struct Map{
        explicit Map(const Cell_group &);
        const Cell& operator [](const Coordinates &) const;
        int find (const Coordinates &) const;
    private:
        std::vector< int> _coordinate_index;
        const Cell_group &_group;
        int base_y, base_x, columns;
        int _index(const Coordinates &) const;
    };
}