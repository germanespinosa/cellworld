#pragma once
#include <chrono>
#define WAIT_UNTIL(EXPRESSION) while(!(EXPRESSION));

namespace cell_world {
    struct Timer {
        Timer(float time_out) : check_point(std::chrono::high_resolution_clock::now()), time(time_out) {}
        Timer(): Timer(0) {};
        void reset() {
            check_point = std::chrono::high_resolution_clock::now();
        }
        float to_seconds() const{
            auto time_stamp = std::chrono::high_resolution_clock::now();
            auto ms = std::chrono::duration<float, std::milli>(time_stamp - check_point).count();
            return ((float) ms) / 1000;
        };
        std::chrono::time_point<std::chrono::high_resolution_clock> check_point;
        float time = 0;
        bool time_out() { return to_seconds() > time;}

        static void wait(float t){
            Timer ts(t);
            while (!ts.time_out());
        }
    };
}