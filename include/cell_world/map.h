#pragma once
#include <cell_world/core.h>
#include <cell_world/cell_group.h>

namespace cell_world{
    struct Map{
        Map() = default;
        explicit Map(const Cell_group &);
        const Cell& operator [](const Coordinates &) const;
        int find (const Coordinates &) const;
        const Cell_group cells;
        Coordinates_list coordinates;
    private:
        std::vector<int> _coordinate_index;
        Coordinates _base;
        int columns, rows;
        int _index(const Coordinates &) const;
    };
}