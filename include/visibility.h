#pragma once
#include<world.h>

namespace cellworld{
    struct Visibility{
        Visibility( World& );
        ~Visibility();
        std::vector<Cell> line (const Cell&, const Cell&) const;
        bool get_visible_cells (Cell_group &, uint32_t) const;
        bool is_visible(const Cell&, const Cell&) const;
        bool is_visible(const uint32_t, const uint32_t) const;
        void reset();
        private:
        bool _get_visibility(const uint32_t, const uint32_t);
        void _set_visibility(const uint32_t, const uint32_t, const uint8_t);
        uint32_t _visibility_index(const uint32_t, const uint32_t) const;
        World &_world;
        uint8_t *_visibility;
    };
}