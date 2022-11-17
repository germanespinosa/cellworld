#pragma once
#include <json_cpp.h>
#include <iostream>
#include <cinttypes>
#include <string>
#include <vector>

namespace cell_world {
    struct Transformation : json_cpp::Json_object {
        Transformation();

        Transformation(float size, float rotation);

        float theta() const;

        float size{};
        float rotation{};

        Json_object_members({
                                Add_member(size);
                                Add_member(rotation);
                            });
    };
}