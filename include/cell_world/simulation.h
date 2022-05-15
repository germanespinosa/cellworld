#pragma once
#include <cell_world/core.h>

namespace cell_world {
    struct Reward : json_cpp::Json_object {
        float step_cost;
        float gamma;
        float capture_cost;
        float episode_reward;
        float default_value;

        Json_object_members(
                Add_member(step_cost);
                Add_member(gamma);
                Add_member(capture_cost);
                Add_member(episode_reward);
                Add_member(default_value);
                )
    };
    using Belief_state_representation = json_cpp::Json_vector<unsigned int>;

}