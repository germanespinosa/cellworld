#pragma once
#include <cell_world/core.h>
#include <cell_world/world.h>
#include <cell_world/agent.h>
#include <cell_world/visibility.h>

namespace cell_world{
    struct Model
    {
        enum class Status{
            Idle,
            Running,
            Stopped,
            Finalized,
        };
        Model(Cell_group &, unsigned int);
        explicit Model(Cell_group &);
        void add_agent(Agent &);
        bool try_update();
        bool update();
        void start_episode();
        void start_episode(unsigned int);
        void end_episode();
        Cell_group cells;
        void run();
        void run(unsigned int);
        Status status;
        bool finished{};
        Map map;
        Graph visibility;
        Model_state state;
    protected:
        std::vector<std::reference_wrapper<Agent>> _agents;
        unsigned int _current_turn;
    };
}