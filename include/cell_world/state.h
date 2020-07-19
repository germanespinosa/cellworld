#pragma once
#include <cell_world/core.h>
#include <cell_world/chance.h>
#include <cell_world/connection.h>
#include <atomic>

namespace cell_world {
    struct Agent_state : json_cpp::Json_object{
        Agent_state();
        Agent_state(unsigned int , Cell);
        unsigned int iteration;
        Cell cell;
        Json_set_builder({
            Json_add_member(iteration,true);
            Json_add_member(cell,true);
        })
    };

    using Agent_state_list = json_cpp::Json_vector<Agent_state>;

    struct Model_state : json_cpp::Json_object{
        unsigned int iterations;
        Agent_state_list agents_state;
        Json_set_builder({
            Json_add_member(iterations,true);
            Json_add_member(agents_state,true);
        })
    };
}