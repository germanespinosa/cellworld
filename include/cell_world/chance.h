#pragma once
#include <cell_world/core.h>
#define CELL_WORLD_CHANCE_MAX RAND_MAX
#define CELL_WORLD_CHANCE rand()
#define pick_random_index(VECTOR) cell_world::Chance::dice(VECTOR.size())
#define pick_random(VECTOR) VECTOR[pick_random_index(VECTOR)]

namespace cell_world{
    struct Chance {
        static unsigned int dice();
        static unsigned int dice(unsigned int);
        static int dice( int,  int);
        static unsigned int pick_random_occurrence(const std::vector<double> &, double);
        static unsigned int pick_random_occurrence(const std::vector<double> &, double, unsigned int);
        static unsigned int pick_by_chance(const std::vector<double> &,const std::vector<unsigned int> &);
        static unsigned int pick_by_chance(const std::vector<double> &,const std::vector<unsigned int> &, unsigned int);
        static unsigned int pick(const std::vector<unsigned int> &);
        static unsigned int pick(const std::vector<unsigned int> &, unsigned int);
        static unsigned int pick_inverse(const std::vector<unsigned int> &);
        static unsigned int pick_inverse(std::vector<unsigned int>, unsigned int);
        static unsigned int pick_best(double, const std::vector<double> &);
        static unsigned int pick_best(double, const std::vector<double> &, unsigned int);
        static std::vector<unsigned int> get_chances(const std::vector<double> &, double, double );
        static std::vector<unsigned int> invert_chances(std::vector<unsigned int>);
        static std::vector<unsigned int> combine_chances(const std::vector<unsigned int>&, const std::vector<unsigned int>&);
        static unsigned int weighted_random(const std::vector<float> &);
        static bool coin_toss(double);
        static bool coin_toss(double, unsigned int);
        static double dice_double (double);
        static double dice_double (double,double);
    };

}