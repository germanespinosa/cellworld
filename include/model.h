#pragma once
#include <ge211.h>
#include <core.h>
#include <world.h>
#include <agent.h>

namespace cell_world{
    struct Model
    {
        Model( World &world, std::vector<Agent*>&);
        bool update();
        std::vector<Agent_data> get_agents_data();
        void start_episode();
        void end_episode();
        protected:
        State state;
        World &_world;
        std::vector<Agent*> &_agents;
        void _epoch ();
        friend class Simulation;
    };
} 