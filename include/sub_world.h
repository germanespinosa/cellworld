#pragma once
#include <core.h>
#include <world.h>
#include <cell_group.h>
#include <ge211.h>
#include <connection.h>

namespace cell_world{
   struct Gate_connection{
        uint32_t gate_id;
        uint32_t sub_world_id;
    };

    struct Gate{
        explicit Gate(uint32_t);
        bool connect(uint32_t);
        bool is_connected(uint32_t);
        std::vector<Gate_connection> gate_connections;
        uint32_t cell_id;
        std::vector<uint32_t> sub_world_ids;
        friend class Sub_worlds;
    };

    struct Sub_world{
        explicit Sub_world(const World &);
        bool add_gate(uint32_t);
        Cell_group cells;
        std::vector<uint32_t>  _gate_ids;
        bool is_connected (uint32_t);
    };

    class Sub_worlds{
        public:
        const static int32_t Occluded = -3;
        const static int32_t Is_gate = -2;
        Sub_worlds();
        Sub_worlds(const World &world, const Cell_group &bridges, const Connections &connections);
        std::vector<Gate> gates;
        void reset (const World &, const Cell_group &, const Connections &);
        Cell_group find_bridges(const World &, const Connections &);
        void reset_connections();
        int32_t get_sub_world_index(uint32_t) const;
        Gate &gate_by_cell_id(uint32_t);
        bool get_cells(Cell_group&, uint32_t) const;
        uint32_t size();

        private:
        std::vector<int32_t> _cell_sub_world_index;
        std::vector<int32_t> _gate_index;
        std::vector<Sub_world> _sub_worlds;
        uint32_t _size;
    };
}