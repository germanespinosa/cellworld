#pragma once
#include <cell_world/world.h>
#include <cell_world/visibility.h>
#include <cell_world/timer.h>

namespace cell_world {

    struct Peeking_parameters : json_cpp::Json_object {
        Json_object_members(
                Add_member(margin);
                Add_member(time_limit);
        );
        float margin{};
        float time_limit{};
    };

    struct Peeking {
        Peeking(const Peeking_parameters &, const World &);
        Location_visibility peeking_visibility;
        Timer peeking_timer;
        bool is_seen(const Location &predator_location, const Location &prey_location);
        void not_visible();
    };
}
