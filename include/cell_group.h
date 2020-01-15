#pragma once
#include <core.h>
#include <ge211.h>

namespace cell_world{
    struct Cell_group{
        Cell_group();
        bool save(const std::string &) const;
        bool save() const;
        bool add(const Cell &);
        bool remove(const Cell &);
        bool toggle(const Cell &);
        bool contains(uint32_t) const;
        bool contains(const Cell &) const;
        void clear();
        uint32_t size() const;
        const Cell &operator [](uint32_t) const;
        int32_t find(uint32_t) const;
        int32_t find(Coordinates) const;
        double distance(uint32_t, uint32_t) const;
        double distance(const Cell&, const Cell&) const;
        Cell_group &operator = (const Cell_group &);
        Cell_group &operator += (const Cell_group &);
    private:
        std::vector<const Cell*> _cells;
        std::vector<int32_t> _id_index;
        std::string _file_name;
        int32_t _coordinate_index[256][256]{};
        const Cell & _get_cell(uint32_t) const;
    };
}