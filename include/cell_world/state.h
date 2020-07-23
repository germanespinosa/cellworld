#pragma once
#include <cell_world/core.h>
#include <cell_world/chance.h>
#include <cell_world/connection.h>
#include <atomic>

namespace cell_world {
    struct Agent_state : json_cpp::Json_object{
        Agent_state();
        Agent_state(unsigned int , Cell);
        unsigned int agent_index;
        unsigned int iteration;
        Cell cell;
        Json_object_members({
            Add_member(agent_index,true);
            Add_member(iteration,true);
            Add_member(cell,true);
        })
    };

    using Agent_state_list = json_cpp::Json_vector<Agent_state>;

    struct Model_state : json_cpp::Json_object{
        enum Status{
            Idle,
            Running,
            Stopped
        };
        Status status;
        unsigned int current_turn;
        unsigned int iterations;
        Agent_state_list agents_state;
        Json_object_members({
            Add_member(status,true);
            Add_member(current_turn,true);
            Add_member(iterations,true);
            Add_member(agents_state,true);
        })
    };
}