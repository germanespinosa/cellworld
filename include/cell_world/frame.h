#pragma once
#include <cell_world/location.h>
#include <cell_world/experiment.h>

namespace cell_world {
    struct Frame : json_cpp::Json_object {
        Json_object_members(
                Add_member(frame_number);
                Add_member(time_stamp);
                Add_member(prey_detection);
                Add_member(predator_detection);
                Add_member(prey_location);
                Add_member(predator_location);
                Add_member(prey_rotation);
                Add_member(predator_rotation);
                Add_member(prey_data);
                Add_member(predator_data);
        )
        unsigned int frame_number;
        float time_stamp;
        bool prey_detection;
        bool predator_detection;
        Location prey_location;
        Location predator_location;
        float prey_rotation;
        float predator_rotation;
        std::string prey_data;
        std::string predator_data;
    };

    struct Frame_list : json_cpp::Json_vector<Frame> {
        Frame_list(size_t s) : json_cpp::Json_vector<Frame>(s) {}
        static Frame_list from_episode (const Episode &);
    };
}