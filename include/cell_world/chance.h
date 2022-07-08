#pragma once
#include <cell_world/core.h>
#define CELL_WORLD_CHANCE_MAX RAND_MAX
#define CELL_WORLD_CHANCE rand()
#define pick_random_index(VECTOR) cell_world::Chance::dice(VECTOR.size())
#define pick_random(VECTOR) VECTOR[pick_random_index(VECTOR)]

namespace cell_world{
    struct Chance {
        static void seed(unsigned int seed);
        static unsigned int dice();
        static unsigned int dice(unsigned int);
        static int dice( int,  int);
        static unsigned int pick_random_occurrence(const std::vector<float> &, float);
        static unsigned int pick_random_occurrence(const std::vector<float> &, float, unsigned int);
        static unsigned int pick_by_chance(const std::vector<float> &,const std::vector<unsigned int> &);
        static unsigned int pick_by_chance(const std::vector<float> &,const std::vector<unsigned int> &, unsigned int);
        static unsigned int pick(const std::vector<unsigned int> &);
        static unsigned int pick(const std::vector<unsigned int> &, unsigned int);
        static unsigned int pick_inverse(const std::vector<unsigned int> &);
        static unsigned int pick_inverse(std::vector<unsigned int>, unsigned int);
        static unsigned int pick_best(float, const std::vector<float> &);
        static unsigned int pick_best(float, const std::vector<float> &, unsigned int);
        static std::vector<unsigned int> get_chances(const std::vector<float> &, float, float );
        static std::vector<unsigned int> invert_chances(std::vector<unsigned int>);
        static std::vector<unsigned int> combine_chances(const std::vector<unsigned int>&, const std::vector<unsigned int>&);
        static unsigned int weighted_random(const std::vector<float> &);
        static bool coin_toss(float);
        static bool coin_toss(float, unsigned int);
        static float dice_float (float);
        static float dice_float (float,float);
    private:
        static unsigned int random();
    };

}