#pragma once
#include <core.h>

namespace cellworld{
     struct Agent_data{
        uint32_t type;
        uint32_t id;
        uint8_t status;
        Color color;
        uint32_t cell_id;
    };
     struct State{
        uint32_t iteration;
        std::vector<const Agent_data*> agents;
    };

    struct Agent{
        virtual void start_episode(const State &) = 0;
        virtual void update_state(const State &) = 0;
        virtual uint32_t action_ready() = 0;
        virtual uint32_t get_destination() = 0;
        virtual void end_episode(const State &) = 0;
        Agent_data data;
        bool active = true;
    };
}  