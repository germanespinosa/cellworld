#include <cell_world/geometry.h>

namespace cell_world{
    float angle_difference(float a1, float a2) {
        a1 = normalize(a1);
        a2 = normalize(a2);
        if (a1 > a2) {
            auto d = a1 - a2;
            if (d < M_PI) return d;
            else return a2 + M_PI * 2.0 - a1;
        } else {
            auto d = a2 - a1;
            if (d < M_PI) return d;
            else return a1 + M_PI * 2.0 - a2;
        }
    }

    bool angle_between(float v, float l1, float l2, bool inclusive){
        if (inclusive) {
            if (angle_difference(v,l1)==0 || angle_difference(v,l2)==0) return true;
        }
        return (direction(v,l1) + direction(v,l2)) == 0;
    }

    int direction(float a1, float a2) {
        a1 = normalize(a1);
        a2 = normalize(a2);
        if (a1 > a2) {
            auto d = a1 - a2;
            if (d < M_PI) return 1;
            else return -1;
        } else {
            auto d = a2 - a1;
            if (d < M_PI) return -1;
            else return 1;
        }
    }

    float normalize(float angle) {
        while (angle < 0) angle += 2.0 * M_PI;
        while (angle > 2 * M_PI) angle -= 2.0 * M_PI;
        return angle;
    }

    float to_radians(float degrees) {
        return degrees / 360.0 * 2.0 * M_PI;
    }

    float to_degrees(float radians) {
        return radians * 360.0 / (2.0 * M_PI);
    }

    float normalize_degrees(float angle) {
        while (angle < -180.0) angle += 360.0;
        while (angle > 180.0) angle -= 360.0;
        return angle;
    }

}