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
}