#pragma once
#include <cell_group.h>
namespace cell_world{
    struct Visibility{
        Visibility ();
        explicit Visibility(const Cell_group &);
        const Cell_group &operator [](const Cell&) const;
        void reset();
        void reset(const Cell_group &);
        double get_entropy();
        private:
        enum Visible{
            Not_checked,
            Visible,
            Not_visible
        };
        std::vector<Cell_group> _visible_cells;
        Cell_group _cell_group;
    };
}