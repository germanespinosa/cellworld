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
        int32_t find(uint32_t) const;
        int32_t find(const Cell &) const;
        double distance(uint32_t, uint32_t) const;
        double distance(const Cell&, const Cell&) const;
        Cell_group random_shuffle() const;
        Cell_group &operator = (const Cell_group &);
        Cell_group &operator += (const Cell_group &);
        Cell_group &operator += (const Cell &);
        Cell_group &operator -= (const Cell_group &);
        Cell_group &operator -= (const Cell &);
        Cell_group operator + (const Cell_group &);
        Cell_group operator + (const Cell &);
        Cell_group operator - (const Cell_group &);
        Cell_group operator - (const Cell &);
        const Cell &operator [](uint32_t) const;
    private:
        std::vector<std::reference_wrapper<const Cell>> _cells;
        std::vector<int32_t> _id_index;
        std::string _file_name;
        const Cell & _get_cell(uint32_t) const;
        const std::string _extension = ".group";
        friend class World;
    };

    struct Map{
        explicit Map(const Cell_group &);
        const Cell& operator [](const Coordinates &) const;
        int32_t find (const Coordinates &) const;

    private:
        int32_t _coordinate_index[256][256]{};
        const Cell_group &_group;
    };
}