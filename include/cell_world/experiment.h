#pragma once
#include <cell_world/cell_group.h>
#include <cell_world/shape.h>

namespace cell_world {
    struct Step : json_cpp::Json_object{
        Json_object_members(
                Add_member(time_stamp);
                Add_member(agent_name);
                Add_member(frame);
                Add_member(location);
                Add_member(rotation);
                Add_optional_member(data);
        );
        float time_stamp;
        std::string agent_name;
        unsigned int frame;
        cell_world::Location location;
        float rotation;
        std::string data;
        Step convert(const Space &src, const Space &dst) const {
            Step converted = *this;
            converted.location = dst.transform(location, src);
            converted.rotation += dst.transformation.rotation - src.transformation.rotation;
            return converted;
        };
        bool operator==(const Step &s) const {
            return this->location == s.location &&
                   this->rotation == s.rotation &&
                   this->data == s.data &&
                   this->agent_name == s.agent_name &&
                   this->frame == s.frame &&
                   this->time_stamp == s.time_stamp;
        }
    };

    struct Trajectories : json_cpp::Json_vector<Step> {
        Trajectories get_agent_trajectory(const std::string &agent_name) const;
        size_t get_step_index_by_frame(unsigned int frame, bool exact = true);
        size_t get_step_index_by_time_stamp(float time_stamp, bool exact = false);
        Step &get_step_by_frame(unsigned int frame, bool exact = true);
        Step &get_step_by_time_stamp(float time_stamp, bool exact = false);
    };

    struct Episode : json_cpp::Json_object {
        Json_object_members(
                Add_member(start_time);
                Add_member(time_stamp);
                Add_member(end_time);
                Add_member(trajectories);
                Add_optional_member(captures);
                Add_optional_member(rewards_sequence);
                Add_optional_member(rewards_time_stamps);
        )
        json_cpp::Json_date start_time;
        float time_stamp;
        json_cpp::Json_date end_time;
        Trajectories trajectories;
        json_cpp::Json_vector<unsigned int> captures;
        Cell_group_builder rewards_sequence;
        Json_float_vector rewards_time_stamps;
        bool operator==(const Episode &s) const {
            return this->start_time == s.start_time &&
                   this->time_stamp == s.time_stamp &&
                   this->end_time == s.end_time;
        }
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
                Add_optional_member(episode_count);
                Add_optional_member(rewards_cells);
                Add_optional_member(rewards_orientations);
        );
        std::string name;
        std::string subject_name;
        std::string world_configuration_name;
        std::string world_implementation_name;
        std::string occlusions;
        Cell_group_builder rewards_cells;
        Json_int_vector rewards_orientations;
        unsigned int duration;
        json_cpp::Json_date start_time;
        Episodes episodes;
        unsigned int episode_count{0};
        void set_name(const std::string &prefix = "", const std::string &suffix = "") {
            name = start_time.to_string("%Y%m%d_%OH%M_") + subject_name + "_" + occlusions;
            if (!prefix.empty()) name = prefix + "_" + name;
            if (!suffix.empty()) name = name + "_" + suffix;
        };
    };
}