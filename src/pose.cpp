#include <cell_world/pose.h>

namespace cell_world {

    Normalized_pose cell_world::Normalized_pose::rotate(float theta) {
        Normalized_pose new_pose = *this;
        Location origin(0, 0);
        for (auto &l: new_pose) {
            auto lt = origin.atan(l);
            auto d = origin.dist(l);
            l = origin.move(lt + theta, d);
        }
        return new_pose;
    }

    Normalized_pose Normalized_pose::move(const Location &nl) {
        Normalized_pose new_pose = *this;
        for (auto &l: new_pose) {
            l += nl;
        }
        return new_pose;
    }

    Normalized_pose::Normalized_pose(const Pose &pose,
                                     unsigned int center_part,
                                     unsigned int rotation_part,
                                     const Json_unsigned_int_vector &part_indexes) {
        auto theta = pose[center_part].location.atan(pose[rotation_part].location);
        auto center = pose[center_part].location;
        auto origin = Location(0,0);
        for (auto i : part_indexes) {
            auto l = pose[i].location - center;
            auto d = l.dist();
            auto t = l.atan() + theta;
            emplace_back(origin.move(t,d));
        }
    }

    Normalized_pose::Normalized_pose(const Pose &pose,
                                     const std::string &center_part_name,
                                     const std::string &rotation_part_name,
                                     const Json_string_vector &part_names) :
            Normalized_pose(pose,
                            pose.find_first_index(
                                    [&center_part_name](const Pose_part &i) {
                                        return i.part == center_part_name;
                                    }
                            ),
                            pose.find_first_index(
                                    [&rotation_part_name](const Pose_part &i) {
                                        return i.part == rotation_part_name;
                                    }
                            ),
                            part_names.process<unsigned int>(
                                    [&pose](auto &p) {
                                        return (unsigned int) pose.find_first_index([&p](const Pose_part &i) {
                                            return i.part == p;
                                        });
                                    }
                            )) {
    }

    Normalized_pose_collection::Normalized_pose_collection(
            const std::string &center_part,
            const std::string &rotation_part,
            const Json_string_vector &part_names) :
            center_part(center_part),
            rotation_part(rotation_part),
            part_names(part_names) {
    }

    bool Normalized_pose_collection::add_pose(const Pose &new_pose, float score_threshold) {
        if (std::any_of(new_pose.begin(),new_pose.end(),[score_threshold](const Pose_part &pp){ return pp.score<score_threshold;})) return false;
        try {
            poses.emplace_back(new_pose, center_part, rotation_part, part_names);
        } catch (...) {
            return false;
        }
        return true;
    }

    bool Normalized_pose_collection::add_normalized_pose(
            const Normalized_pose &new_normalized_pose) {
        if (new_normalized_pose.size() != part_names.size()) return false;
        poses.push_back(new_normalized_pose);
        return true;
    }
}