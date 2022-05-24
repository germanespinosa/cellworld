#pragma once
#include <cell_world/core.h>
#include <cell_world/world.h>
#include <cell_world/visibility.h>
#include <cell_world/timer.h>

namespace cell_world {
    struct Capture_parameters : json_cpp::Json_object {
        Json_object_members(
                Add_member(distance);
                Add_member(angle);
                Add_member(cool_down);
        );
        double distance{};
        double angle{};
        double cool_down;
    };

    struct Capture {
        Capture(const Capture_parameters &, const World &);
        double dist_max;
        double theta_diff_max;
        Timer cool_down;

        Location_visibility visibility;
        bool is_captured(const Location &predator_location, double predator_theta, const Location &prey_location);
        bool is_captured(const Location &predator_location, const Location &prey_location);
    };
}
