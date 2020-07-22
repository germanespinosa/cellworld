#pragma once
#include <cell_world/core.h>
#include <cell_world/world.h>
#include <cell_world/agent.h>
#include <cell_world/visibility.h>
#include <cell_world/map.h>

namespace cell_world{
    struct Model
    {
        Model(Cell_group &, unsigned int);
        explicit Model(Cell_group &);
        void add_agent(Agent &);
        bool update();
        void start_episode();
        void end_episode();
        void run();
        Map map;
        Model_state state;
    protected:
        std::vector<std::reference_wrapper<Agent>> _agents;
    };
}