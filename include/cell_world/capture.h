#pragma once
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
        float distance{};
        float angle{};
        float cool_down;
    };

    struct Capture {
        Capture(const Capture_parameters &, const World &);
        float dist_max;
        float theta_diff_max;
        Timer cool_down;
        Location_visibility visibility;
        bool is_captured(const Location &predator_location, float predator_theta, const Location &prey_location);
    };

    struct Cell_capture {
        Cell_capture (const Capture_parameters &, const Graph &);
        const Graph &visibility;
        unsigned int dist_max;
        bool is_captured(const Cell &, const Cell &);
    };
}
