#pragma once
#include <cell_world/cell_group.h>

namespace cell_world{
    struct Map{
        Map() = default;
        explicit Map(const Cell_group &);
        const Cell& operator [](const Coordinates &) const;
        int find (const Coordinates &) const;
        Cell_group cells;
    private:
        std::vector<int> _coordinate_index;
        Coordinates _base;
        int _columns, _rows;
        int _index(const Coordinates &) const;
    };
}