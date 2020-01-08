#pragma once
#include <core.h>
#include <ge211.h>
#include "world.h"
#include "connection.h"


namespace cellworld{
    struct Cell_group{
        explicit Cell_group(const World &);
        bool load(const std::string &);
        bool load();
        bool save(const std::string &) const;
        bool save() const;
        bool add(uint32_t);
        bool remove(uint32_t);
        bool toggle(uint32_t);
        bool contains(uint32_t) const;
        void clear();
        uint32_t size() const;
        const Cell &operator [](const uint32_t) const;
        int32_t find(uint32_t) const;
        int32_t find(Coordinates) const;
        void get_connections(Connections &, const std::vector<Coordinates>) const;
        Cell_group &operator = (Cell_group &);
        Cell_group &operator += (Cell_group &);
    private:
        std::vector<uint32_t> _cell_ids;
        std::vector<int32_t> _index;
        const World &_world;
        std::string _file_name;
        const Cell & _get_cell(uint32_t) const;
    };
}