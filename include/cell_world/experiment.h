#pragma once
#include <cell_world/core.h>
#include <cell_world/cell_group.h>

namespace cell_world {
    struct Step : json_cpp::Json_object{
        Json_object_members(
                Add_member(time_stamp);
                Add_member(agent_name);
                Add_member(frame);
                Add_member(coordinates);
                Add_member(location);
                Add_member(rotation);
                Add_optional_member(data);
        );
        double time_stamp;
        std::string agent_name;
        unsigned int frame;
        cell_world::Coordinates coordinates;
        cell_world::Location location;
        double rotation;
        std::string data;
    };

    using Trajectories = json_cpp::Json_vector<Step> ;

    struct Episode : json_cpp::Json_object {
        Json_object_members(
                Add_member(start_time);
                Add_member(time_stamp);
                Add_member(end_time);
                Add_member(trajectories);
        )
        json_cpp::Json_date start_time;
        double time_stamp;
        json_cpp::Json_date end_time;
        Trajectories trajectories;
    };

    using Episodes = json_cpp::Json_vector<Episode>;

    struct Experiment : json_cpp::Json_object {
        Json_object_members(
                Add_member(name);
                Add_member(subject_name);
                Add_member(world_configuration_name);
                Add_member(world_implementation_name);
                Add_member(occlusions);
                Add_member(duration);
                Add_member(start_time);
                Add_optional_member(episodes);
        );
        std::string name;
        std::string subject_name;
        std::string world_configuration_name;
        std::string world_implementation_name;
        std::string occlusions;
        unsigned int duration;
        json_cpp::Json_date start_time;
        Episodes episodes;
    };

}