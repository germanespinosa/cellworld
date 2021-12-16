#pragma once
#include <chrono>

namespace cell_world {
    struct Timer {
        Timer(double time_out) : check_point(std::chrono::high_resolution_clock::now()), time(time_out) {}
        Timer(): Timer(0) {};
        void reset() {
            check_point = std::chrono::high_resolution_clock::now();
        }
        double to_seconds() const{
            auto time_stamp = std::chrono::high_resolution_clock::now();
            auto ms = std::chrono::duration<double, std::milli>(time_stamp - check_point).count();
            return ((double) ms) / 1000;
        };
        std::chrono::time_point<std::chrono::high_resolution_clock> check_point;
        double time = 0;
        bool time_out() { return time > 0 && to_seconds() > time;}

        static void wait(double t){
            Timer ts(t);
            while (!ts.time_out());
        }
    };
}