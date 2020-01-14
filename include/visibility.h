#pragma once
#include <world.h>
#include <cell_group.h>

namespace cell_world{
    struct Visibility{
        explicit Visibility( World& );
        const Cell_group &operator [](const Cell&) const;
        void reset();
        double get_entropy();
        private:
        enum Visible{
            Not_checked,
            Visible,
            Not_visible
        };
        std::vector<Cell> _line (const Cell&, const Cell&) const;
        bool _get_visibility(uint32_t, uint32_t);
        void _set_visibility(uint32_t, uint32_t, uint8_t);
        uint32_t _visibility_index(uint32_t, uint32_t) const;
        bool _get_visible_cells (Cell_group &, uint32_t) const;
        World &_world;
        uint8_t *_visibility;
        std::vector<Cell_group> _visible_cells;
    };
}