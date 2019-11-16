#pragma once
#include<world.h>

namespace cellworld{
    struct Visibility{
        Visibility( World& );
        std::vector<Cell> line (const Cell&, const Cell&) const;
        std::vector<Cell> visible_cells (const Cell&) const;
        bool is_visible(const Cell&, const Cell&) const;
        private:
        bool _get_visibility(const uint32_t, const uint32_t);
        void _set_visibility(const uint32_t, const uint32_t, const uint8_t);
        uint32_t _visibility_index(const uint32_t, const uint32_t) const;
        World &_world;
        uint8_t *_visibility;
    };
}