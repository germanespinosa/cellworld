#pragma once
#include <core.h>
#include <world.h>
#include <agent.h>
#include <visibility.h>

namespace cell_world{
    struct Model
    {
        enum class Mode{
            Simultaneous,
            Turns
        };
        enum class Status{
            Idle,
            Running,
            Stopped,
            Finalized,
        };
        Model(Cell_group &, uint32_t);
        explicit Model(Cell_group &);
        void add_agent(Agent &);
        bool try_update();
        bool update();
        std::vector<Agent_data> get_agents_data();
        void start_episode();
        void start_episode(uint32_t);
        void end_episode();
        State get_state(uint32_t);
        State get_state();
        uint32_t iteration;
        uint32_t iterations;
        Cell_group cells;
        void run();
        void run(uint32_t);
        Status status;
        bool finished{};
        Mode mode;
        History history;
        Map map;
        Graph visibility;
    protected:
        std::vector<std::vector<Coordinates>> trajectories;
        std::vector<std::vector<double>> values;
        std::vector<std::reference_wrapper<Agent>> _agents;
        uint32_t _message_group;
        uint32_t _current_turn;
        bool _try_update_simultaneous();
        bool _try_update_turn();
    friend class Simulation;
    };

} 