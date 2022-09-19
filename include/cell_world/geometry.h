#pragma once
#include <json_cpp.h>
#include <iostream>
#include <cinttypes>
#include <string>
#include <vector>
#include <cell_world/location.h>

namespace cell_world {
    float angle_difference(float, float);
    int direction(float, float);
    bool angle_between(float value, float lim1, float lim2, bool inclusive = false);
    float to_radians(float);
    float to_degrees(float);
    float normalize(float);
    float normalize_degrees(float);
    bool segments_intersect(const Location &segment1_point1, const Location &segment1_point2, const Location &segment2_point1, const Location &segment2_point2);
}