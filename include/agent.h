#pragma once
#include <core.h>

namespace cellworld{
     struct Agent_data{
        uint32_t id = 0;
        uint32_t iteration = 0;
        Coordinates coordinates {0,0};
        uint8_t status = 0;
        Color color = Black;
    };
     struct State{
        uint32_t iteration;
        std::vector<Agent_data> agents;
    };
    struct Action{
        uint32_t iteration;
        Coordinates action;
    };
    struct Agent{
         virtual void update_state(State) = 0;
         virtual bool get_action() = 0; 
         Action action;
         Agent_data data;
    };
}  