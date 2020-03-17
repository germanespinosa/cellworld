#include <chance.h>
#include <core.h>
#include <fstream>
#include <iostream>
#include <utility>

using namespace cell_world;
using namespace std;

uint32_t pick(std::vector<uint32_t> _chances, std::vector<double> &values, uint32_t dice) {
    auto indexes = new_index(values.size()); // creates an index vector for the options

    for (uint32_t i = 1; i < indexes.size(); i++) // sort the indexes of the options by expected reward descending
        for (uint32_t j = i; j > 0 && values[indexes[j - 1]] < values[indexes[j]]; j--)
            swap(indexes[j - 1], indexes[j]);

    uint32_t action;
    uint32_t max = _chances[values.size()-1];
    uint32_t chance_dice = (dice % max) + 1;
    for (action = 0 ; chance_dice > _chances[action] && action < values.size()-1; action++);
    double reward = values[indexes[action]]; // this is the expected value of the state-action pair

    uint32_t repetitions = 0;
    for (double value : values) if (value == reward) repetitions ++; //find how many times the same value appears

    uint32_t selection = dice % repetitions; //use the dice to determine which repetition to use
    uint32_t pick;
    for (pick = 0; values[pick] != reward || selection--; pick++); // find the correct repetition

    return pick; //pick the correct one
}

uint32_t Chance::dice(uint32_t max) {
    return dice() % max;
}

int32_t Chance::dice(int32_t min, int32_t max) {
    return (dice() % (max-min+1)) + min;
}

uint32_t Chance::dice() {
    return CELL_WORLD_CHANCE;
}

double Chance::dice_double(double max) {
    return (double)dice() * max / CELL_WORLD_CHANCE_MAX;
}

double Chance::dice_double(double min,double max) {
    return (double)dice() * (max-min) / CELL_WORLD_CHANCE_MAX + min;
}

uint32_t Chance::pick_best(double probability_best, const std::vector<double>&values) {
    return pick_best(probability_best,values, Chance::dice());
}

bool Chance::coin_toss(double probability) {
    return coin_toss(probability, Chance::dice());

}

bool Chance::coin_toss(double probability, uint32_t dice) {
    if (probability>1) throw logic_error("Chance::coin_toss - probability can't be larger than 1");
    dice = dice % CELL_WORLD_CHANCE_MAX;
    return (dice<(probability*CELL_WORLD_CHANCE_MAX));
}

uint32_t Chance::pick_best(double probability_best, const std::vector<double> &values, uint32_t dice) {
    if (values.empty()) throw logic_error("Chance::pick_best - can't pick from an empty set.");
    uint32_t size = values.size();
    if (coin_toss(probability_best, dice)){ // find one random item from the best values
        double m = max(values);
        return Chance::pick_random_occurrence(values, m, dice);
    } else { // return a random item
        return dice % size;
    }
}

uint32_t Chance::pick(const std::vector<uint32_t> &chances) {
    return Chance::pick(chances, Chance::dice());
}

uint32_t Chance::pick(const std::vector<uint32_t> &chances, uint32_t dice) {
    if (chances.empty()) throw logic_error("Chance::pick - can't pick from an empty set.");
    uint32_t sum = 0;
    for (auto c:chances) sum+=c;
    uint32_t size = chances.size();
    if (sum==0) return dice % size;
    dice = dice % sum;
    sum=0;
    for(uint32_t i=0;i<size-1;i++) {
        sum+=chances[i];
        if (sum>dice) return i;
    }
    return size-1;
}

uint32_t Chance::pick_inverse(const std::vector<uint32_t> &chances) {
    return Chance::pick_inverse(chances, Chance::dice());
}

uint32_t Chance::pick_inverse(std::vector<uint32_t> chances, uint32_t dice) {
    if (chances.empty()) throw logic_error("Chance::pick_inverse - can't pick from an empty set.");
    uint32_t min = chances[0];
    uint32_t max = chances[0];
    for(auto &c:chances) {
        if (c < min) min = c; else if (c > max) max = c;
    }
    uint32_t target = min + max;
    for (auto &c:chances ){
        c = target - c;
    }
    return pick(chances, dice);
}

uint32_t Chance::pick_by_chance(const std::vector<double> &values, const std::vector<uint32_t> &chances) {
    return Chance::pick_by_chance(values, chances, Chance::dice());
}

uint32_t Chance::pick_random_occurrence(const std::vector<double> &values, double value, uint32_t dice) {
    uint32_t occurrences = 0;
    for (double v : values) if (v == value) occurrences ++; //find how many times the same value appears
    if (occurrences == 0) throw logic_error("Chance::pick_random_occurrence - value not found");
    uint32_t selected = dice % occurrences;
    uint32_t pick = 0;
    for (pick = 0; values[pick] != value || selected--; pick++); // find the correct repetition
    return pick;
}

uint32_t Chance::pick_by_chance(const std::vector<double> &values, const std::vector<uint32_t> &chances, uint32_t dice) {
    auto index = new_index(values, false); // creates an sorted index for the values
    uint32_t action = Chance::pick(chances,dice); // gets the selected position
    if (action>=values.size()) action=values.size()-1; // if it has more chances than values
    double selected_value = values[index[action]]; // this is the selected value
    return Chance::pick_random_occurrence(values,selected_value, dice); // find a random occurrence of the value in the vector
}

uint32_t Chance::pick_random_occurrence(const std::vector<double> &values, double value) {
    return Chance::pick_random_occurrence(values, value,Chance::dice());
}
