#pragma once
#include <cell_world/core.h>
#include <cell_world/world.h>
#include <cell_world/visibility.h>

namespace cell_world {

    struct Peeking_parameters : json_cpp::Json_object {
        Json_object_members(
                Add_member(margin);
                Add_member(time_limit);
        );
        double margin;
        double time_limit;
    };

    struct Capture_parameters : json_cpp::Json_object {
        Json_object_members(
                Add_member(distance);
                Add_member(angle);
        );
        double distance{};
        double angle{};
    };

    struct Capture {
        Capture(const Capture_parameters &, const World &);
        double dist_max;
        double theta_diff_max;
        Location_visibility visibility;
        bool is_captured( const Location &predator_location, double predator_theta, const Location &prey_location);
    };
}
