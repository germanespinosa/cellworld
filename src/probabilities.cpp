#include <probabilities.h>
using namespace cell_world;
using namespace std;

uint32_t Probabilities::size() {
    return _chances.size();
}

double Probabilities::probability(uint32_t index) {
    return _get_probability(index,max());
}

uint32_t Probabilities::operator[](uint32_t index) {
    if (index==0) return _chances[index];
    return _chances[index] - _chances[index-1];
}

Probabilities::Probabilities(const std::vector<uint32_t>& individual_chances) {
    uint32_t accumulated = 0;
    for ( uint32_t ic:individual_chances ){
        _chances.push_back(ic + accumulated );
        accumulated += ic;
    }
}

Probabilities::Probabilities(const std::vector<double>&individual_probabilities) {
    double accumulated = 0;
    for ( double ic:individual_probabilities ){
        _chances.push_back( ( ic + accumulated ) * 100 );
        accumulated += ic;
    }
}

uint32_t Probabilities::pick(std::vector<double> values, uint32_t dice) {
    vector<uint32_t> indexes(values.size()); // creates an index vector for the options
    for (uint32_t i = 0;i < values.size();i++) {
        indexes[i] = i;
    }
    for (uint32_t i = 1; i < values.size(); i++) // sort the indexes of the options by expected reward descending
        for (uint32_t j = i; j > 0 && values[indexes[j - 1]] < values[indexes[j]]; j--)
            swap(indexes[j - 1], indexes[j]);

    uint32_t action;
    uint32_t chance_dice = (dice % max()) + 1;
    for (action = 0 ; chance_dice > _chances[action] && action < values.size()-1; action++);
    double reward = values[indexes[action]]; // this is the expected value of the state-action pair

    uint32_t repetitions = 0;
    for (uint32_t i = 0; i < values.size(); i++) if (values[i] == reward) repetitions ++; //find how many times the same value appears

    uint32_t selection = dice % repetitions; //use the dice to determine which repetition to use
    uint32_t pick;
    for (pick = 0; values[pick] != reward || selection--; pick++); // find the correct repetition

    return pick; //pick the correct one
}

uint32_t Probabilities::max() {
    return _chances[_chances.size()-1];
}

double Probabilities::compute(std::vector<double>values) {
    double r = 0;
    for (uint32_t i = 0; i < values.size() && i < _chances.size();i++ ) r+=values[i] * _get_probability(i,_chances[values.size()-1]);
    return r;
}

double Probabilities::_get_probability(uint32_t index, uint32_t max) {
    uint32_t chances = (*this)[index];
    return (double)chances / (double)max;
}

uint32_t Probabilities::pick(uint32_t dice) {
    uint32_t pick;
    uint32_t chance_dice = (dice % max()) + 1;
    for (pick = 0 ; chance_dice > _chances[pick] && pick < _chances.size()-1; pick++);
    return pick;
}

uint32_t Probabilities::dice() {
    return (rand() % max()) +1;
}

uint32_t Probabilities::pick(std::vector<double> values) {
    return pick(values, dice());
}

uint32_t Probabilities::pick() {
    return pick(dice());
}
