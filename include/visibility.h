#pragma once
#include <world.h>
#include <cell_group.h>

namespace cell_world{
    struct Visibility{
        explicit Visibility( World& );
        ~Visibility();
        std::vector<Cell> line (const Cell&, const Cell&) const;
        bool get_visible_cells (Cell_group &, uint32_t) const;
        bool is_visible(const Cell&, const Cell&) const;
        bool is_visible(uint32_t, uint32_t) const;
        void reset();
        private:
        enum Visible{
            Not_checked,
            Visible,
            Not_visible
        };
        bool _get_visibility(uint32_t, uint32_t);
        void _set_visibility(uint32_t, uint32_t, uint8_t);
        uint32_t _visibility_index(uint32_t, uint32_t) const;
        World &_world;
        uint8_t *_visibility;
    };
}