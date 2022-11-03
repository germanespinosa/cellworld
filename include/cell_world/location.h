#pragma once
#include <cell_world/transformation.h>

namespace cell_world {
    struct Location : json_cpp::Json_object {
        Location();
        Location(float x, float y);
        float x{}, y{};
        bool operator==(const Location &) const;
        bool operator!=(const Location &) const;
        Location operator+=(const Location &);
        Location operator+(const Location &) const;
        Location operator-(const Location &) const;
        Location operator-() const;
        Location operator*(float) const;
        Location operator*(const Location &) const;
        Location operator/(float) const;
        float mod() const;
        float atan() const;
        Transformation transformation() const;
        Transformation transformation(const Location &) const;
        float atan(const Location &) const;
        float dist(const Location &) const;
        float dist() const;
        float dist(const Location &, const Location &) const;
        Location orthogonal(const Location &, const Location &) const;
        float manhattan(const Location &) const;
        Location move(float theta, float dist) const;
        void transform (const Transformation &);
        Json_object_members({
            Add_member(x);
            Add_member(y);
        })
    };

    using Location_list = json_cpp::Json_vector<Location>;

}