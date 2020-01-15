#pragma once
#include <core.h>
#include <probabilities.h>
namespace cell_world{

    enum Agent_status{
        active,
        waiting,
        inactive,
        finished
    };

    struct Agent_action{
        Connection_pattern possible_actions;
        Probabilities probabilities;
    };

    struct Agent_data{
        uint32_t type;
        uint32_t id;
        Agent_status status;
        Color color;
        Cell &cell;
    };

    struct State{
        uint32_t iteration;
        std::vector<Agent_data> agents_data;
    };

    struct Agent{
        virtual void start_episode(const State &) = 0;
        virtual void update_state(const State &) = 0;
        virtual uint32_t action_ready() = 0;
        virtual Agent_action &get_action() = 0;
        virtual void end_episode(const State &) = 0;
        Agent_data data;
        bool active = true;
    };
}  