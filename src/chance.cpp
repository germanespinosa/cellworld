#include <cell_world/chance.h>
#include <cell_world/core.h>
#include <iostream>
#include <utility>
#include <mutex>

using namespace std;

namespace cell_world {

    thread_local unsigned int rseq = 1;

    void Chance::seed(unsigned int seed){
        rseq = seed;
    }

    unsigned int pick(std::vector<unsigned int> _chances, std::vector<float> &values, unsigned int dice) {
        auto indexes = new_index(values.size()); // creates an index vector for the options

        for (unsigned int i = 1; i < indexes.size(); i++) // sort the indexes of the options by expected reward descending
            for (unsigned int j = i; j > 0 && values[indexes[j - 1]] < values[indexes[j]]; j--)
                swap(indexes[j - 1], indexes[j]);

        unsigned int action;
        unsigned int max = _chances[values.size() - 1];
        unsigned int chance_dice = (dice % max) + 1;
        for (action = 0; chance_dice > _chances[action] && action < values.size() - 1; action++);
        float reward = values[indexes[action]]; // this is the expected value of the public_state-action pair

        unsigned int repetitions = 0;
        for (float value : values) if (value == reward) repetitions++; //find how many times the same value appears

        unsigned int selection = dice % repetitions; //use the dice to determine which repetition to use
        unsigned int pick;
        for (pick = 0; values[pick] != reward || selection--; pick++); // find the correct repetition

        return pick; //pick the correct one
    }

    unsigned int Chance::dice(unsigned int max) {
        if (max == 0) throw logic_error("Chance::dice - max can't be 0");
        return dice() % max;
    }

     int Chance::dice( int min,  int max) {
        return (dice() % (max - min + 1)) + min;
    }

    //mutex rmutex;

    unsigned int Chance::dice() {
        //rmutex.lock();
        srand(rseq);
        rseq = CELL_WORLD_CHANCE;
        //rmutex.unlock();
        return rseq;
    }

    float Chance::dice_float(float max) {
        return (float) dice() * max / CELL_WORLD_CHANCE_MAX;
    }

    float Chance::dice_float(float min, float max) {
        return (float) dice() * (max - min) / CELL_WORLD_CHANCE_MAX + min;
    }

    bool Chance::coin_toss(float probability) {
        return coin_toss(probability, Chance::dice());

    }

    bool Chance::coin_toss(float probability, unsigned int dice) {
        if (probability == 1) return true;
        if (probability == 0) return false;
        if (probability > 1) throw logic_error("Chance::coin_toss - probability can't be higher than 1");
        if (probability < 0) throw logic_error("Chance::coin_toss - probability can't be lower than 0");
        dice = dice % CELL_WORLD_CHANCE_MAX;
        return (dice < (probability * CELL_WORLD_CHANCE_MAX));
    }

    unsigned int Chance::pick_best(float probability_best, const std::vector<float> &values) {
        return pick_best(probability_best, values, Chance::dice());
    }

    unsigned int Chance::pick_best(float probability_best, const std::vector<float> &values, unsigned int dice) {
        if (values.empty()) throw logic_error("Chance::pick_best - can't pick from an empty set.");
        unsigned int size = values.size();
        if (coin_toss(probability_best, dice)) { // find one random item from the best values
            float m = max(values);
            return Chance::pick_random_occurrence(values, m, dice);
        } else { // return a random item
            return dice % size;
        }
    }

    unsigned int Chance::pick(const std::vector<unsigned int> &chances) {
        return Chance::pick(chances, Chance::dice());
    }

    unsigned int Chance::pick(const std::vector<unsigned int> &chances, unsigned int dice) {
        if (chances.empty()) throw logic_error("Chance::pick - can't pick from an empty set.");
        unsigned int sum = 0;
        for (auto c:chances) sum += c;
        unsigned int size = chances.size();
        if (sum == 0) return dice % size;
        dice = dice % sum;
        sum = 0;
        for (unsigned int i = 0; i < size - 1; i++) {
            sum += chances[i];
            if (sum > dice) return i;
        }
        return size - 1;
    }

    unsigned int Chance::pick_inverse(const std::vector<unsigned int> &chances) {
        return Chance::pick_inverse(chances, Chance::dice());
    }

    unsigned int Chance::pick_inverse(std::vector<unsigned int> chances, unsigned int dice) {
        if (chances.empty()) throw logic_error("Chance::pick_inverse - can't pick from an empty set.");
        chances = invert_chances(chances);
        return pick(chances, dice);
    }

    unsigned int Chance::pick_by_chance(const std::vector<float> &values, const std::vector<unsigned int> &chances) {
        return Chance::pick_by_chance(values, chances, Chance::dice());
    }

    unsigned int Chance::pick_random_occurrence(const std::vector<float> &values, float value, unsigned int dice) {
        unsigned int occurrences = 0;
        for (auto v : values) if (v == value) occurrences++; //find how many times the same value appears
        if (occurrences == 0) throw logic_error("Chance::pick_random_occurrence - value not found");
        unsigned int selected = dice % occurrences;
        unsigned int pick = 0;
        for (pick = 0; values[pick] != value || selected--; pick++); // find the correct repetition
        return pick;
    }

    unsigned int Chance::pick_random_occurrence(const std::vector<float> &values, float value) {
        return Chance::pick_random_occurrence(values, value, Chance::dice());
    }

    unsigned int
    Chance::pick_by_chance(const std::vector<float> &values, const std::vector<unsigned int> &chances, unsigned int dice) {
        auto index = new_index(values, false); // creates a sorted index for the values
        unsigned int action = Chance::pick(chances, dice); // gets the selected position
        if (action >= values.size()) action = values.size() - 1; // if it has more chances than values
        float selected_value = values[index[action]]; // this is the selected value
        return Chance::pick_random_occurrence(values, selected_value,
                                              dice); // find a random occurrence of the value in the vector
    }

    std::vector<unsigned int> Chance::get_chances(const std::vector<float> &values, float min, float max) {
        // values could be negative
        if (values.empty()) return vector<unsigned int>();
        std::vector<unsigned int> chances(values.size());
        for (unsigned int i = 0; i < chances.size(); i++) {
            float new_value = (values[i] - min) / (max - min) * 1000;
            chances[i] = new_value;
        }
        return chances;
    }

    std::vector<unsigned int> Chance::invert_chances(std::vector<unsigned int> chances) {
        if (chances.empty()) throw logic_error("Chance::invert_chances - can't invert empty chances");
        unsigned int min = chances[0];
        unsigned int max = chances[0];
        for (auto &c:chances) {
            if (c < min) min = c; else if (c > max) max = c;
        }
        unsigned int target = min + max;
        for (auto &c:chances) {
            c = target - c;
        }
        return chances;
    }

    std::vector<unsigned int>
    Chance::combine_chances(const std::vector<unsigned int> &chances1, const std::vector<unsigned int> &chances2) {
        if (chances1.size() != chances2.size())
            throw logic_error("Chance::combine_chances - can't combine chances of different size.");
        std::vector<float> values(chances1.size());
        float max = 0;
        for (unsigned int i = 0; i < values.size(); i++) {
            values[i] = (float) chances1[i] * (float) chances2[i];
            if (max < values[i]) max = values[i];
        }
        std::vector<unsigned int> chances(values.size());
        for (unsigned int i = 0; i < values.size(); i++) {
            float new_value = values[i] / max * 1000;
            chances[i] = new_value;
        }
        return chances;
    }

    unsigned int Chance::weighted_random (const std::vector<float> &weights){
        float s = 0;
        for(auto f : weights) s += f;
        auto r = Chance::dice_float(s);
        s = 0;
        unsigned int i=0;
        for (; s+weights[i]<r; i++) s+=weights[i];
        return i;
    }
}