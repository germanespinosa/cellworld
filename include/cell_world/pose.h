#pragma once
#include <json_cpp.h>
#include <cell_world/location.h>
#include <cell_world/types.h>

namespace cell_world {
    struct Pose_part: json_cpp::Json_object {
        Json_object_members(
            Add_member(part);
            Add_member(location);
            Add_member(camera);
            Add_member(score);
        )
        std::string part{};
        Location location{};
        int camera{};
        float score{};
        bool operator == (const Pose_part &o) const {
            return camera == o.camera && location==o.location && part==o.part && score == o.score;
        }
    };

    using Pose = json_cpp::Json_vector<Pose_part>;

    struct Normalized_pose : json_cpp::Json_vector<Location> {
        Normalized_pose() = default;
        Normalized_pose(const Pose &pose,
                        const std::string &center_part_name,
                        const std::string &rotation_part_name,
                        const Json_string_vector &part_names);
        Normalized_pose(const Pose &pose,
                        unsigned int center_part,
                        unsigned int rotation_part,
                        const Json_unsigned_int_vector &part_indexes);
        Normalized_pose rotate(float);
        Normalized_pose move(const Location &);
    };

    struct Normalized_pose_collection: json_cpp::Json_object {
        Json_object_members(
                Add_member(center_part);
                Add_member(rotation_part);
                Add_member(part_names);
                Add_member(poses);
        )
        Normalized_pose_collection() = default;
        Normalized_pose_collection(const std::string &center_part,
                                   const std::string &rotation_part,
                                   const Json_string_vector &part_names);
        std::string center_part;
        std::string rotation_part;
        Json_string_vector part_names;
        json_cpp::Json_vector<Normalized_pose> poses;
        bool add_pose(const Pose &, float threshold = .8);
        bool add_normalized_pose(const Normalized_pose &);
    };
}