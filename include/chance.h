#pragma once
#include <core.h>
#define CELL_WORLD_CHANCE_MAX RAND_MAX
#define CELL_WORLD_CHANCE rand()
namespace cell_world{
    struct Chance {
        static uint32_t dice();
        static uint32_t dice(uint32_t);
        static int32_t dice(int32_t, int32_t);
        static uint32_t pick_random_occurrence(const std::vector<double> &, double);
        static uint32_t pick_random_occurrence(const std::vector<double> &, double, uint32_t);
        static uint32_t pick_by_chance(const std::vector<double> &,const std::vector<uint32_t> &);
        static uint32_t pick_by_chance(const std::vector<double> &,const std::vector<uint32_t> &, uint32_t);
        static uint32_t pick(const std::vector<uint32_t> &);
        static uint32_t pick(const std::vector<uint32_t> &, uint32_t);
        static uint32_t pick_inverse(const std::vector<uint32_t> &);
        static uint32_t pick_inverse(std::vector<uint32_t>, uint32_t);
        static uint32_t pick_best(double, const std::vector<double> &);
        static uint32_t pick_best(double, const std::vector<double> &, uint32_t);
        static bool coin_toss(double);
        static bool coin_toss(double, uint32_t);
        static double dice_double (double);
        static double dice_double (double,double);
    };

}