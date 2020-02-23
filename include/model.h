#pragma once
#include <core.h>
#include <world.h>
#include <agent.h>
#include <visibility.h>

namespace cell_world{
    struct Model
    {
        explicit Model( Cell_group &);
        void add_agent(Agent &);
        bool update();
        std::vector<Agent_data> get_agents_data();
        void start_episode();
        void end_episode();
        State get_state(uint32_t);
        State get_state();
        void set_state(State);
        uint32_t iteration;
        Cell_group cells;
        protected:
        std::vector<Agent*> _agents;
        Map _map;
        Graph _visibility;
        void _epoch();
    };
} 