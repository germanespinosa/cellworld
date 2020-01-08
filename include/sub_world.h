#pragma once
#include <core.h>
#include <world.h>
#include <cell_group.h>
#include <ge211.h>
#include <connection.h>

namespace cellworld{
   struct Gate_connection{
        uint32_t gate_id;
        uint32_t sub_world_id;
    };

    struct Gate{
        Gate(uint32_t);
        bool connect(uint32_t);
        bool is_connected(uint32_t);
        std::vector<Gate_connection> gate_connections;
        uint32_t cell_id;
        std::vector<uint32_t> sub_world_ids;
        friend class Sub_worlds;
    };

    struct Sub_world{
        Sub_world(const World &);
        bool add_gate(uint32_t);
        Cell_group cells;
        std::vector<uint32_t>  _gate_ids;
        bool is_connected (uint32_t);
    };

    struct Sub_worlds{
        const static int32_t Is_gate = -2;
        const static int32_t Occluded = -3;
        Sub_worlds();
        Sub_worlds(const World &world, const Cell_group &bridges, const Connections &connections);
        uint32_t size();
        void reset (const World &, const Cell_group &, const Connections &);
        void reset_connections();
        int32_t get_sub_world_index(uint32_t) const;
        bool get_cells(Cell_group&, uint32_t) const;
        Cell_group find_bridges(const World &, const Connections &);
        Gate &gate_by_cell_id(uint32_t);
        std::vector<Gate> gates;
    private:
        std::vector<int32_t> _cell_sub_world_index;
        std::vector<int32_t> _gate_index;
        std::vector<Sub_world> _sub_worlds;
        uint32_t _size;
    };
}