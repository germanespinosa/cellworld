#pragma once
#include <core.h>
#include <world.h>
#include <cell_group.h>
#include <connection.h>

namespace cell_world{
    struct Sub_world{
        Cell_group cells;
        Cell_group gate_cells;
        bool save(const std::string&);
    };

    struct Gate_connection{
        const Cell &destination;
        const Sub_world &sub_world;
    };

    struct Gate{
        explicit Gate(const Cell &cell);
        const Cell &cell;
        std::vector<Gate_connection> gate_connections;
    };

    class Sub_worlds{
        public:
        const static int32_t Occluded = -3;
        const static int32_t Is_gate = -2;
        Sub_worlds(const Cell_group &);
        Sub_worlds(const Cell_group &, const Cell_group &, const Connections &);
        void reset (const Cell_group &, const Connections &);
        Cell_group find_bridges(const Connections &);
        int32_t get_sub_world_index(uint32_t) const;
        Gate &gate_by_cell_id(uint32_t);
        uint32_t size();
        const Sub_world &operator[](uint32_t) const;
        std::vector<Gate> gates;
        private:
        std::vector<int32_t> _cell_sub_world_index;
        std::vector<int32_t> _gate_index;
        std::vector<Sub_world> _sub_worlds;
        const Cell_group &_cells;
    };
}