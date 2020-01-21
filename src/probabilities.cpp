#include <probabilities.h>
#include <core.h>
#include <fstream>
#include <iostream>

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
        accumulated += ic;
        _chances.push_back(accumulated);
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
    return dice(max());
}

uint32_t Probabilities::pick(std::vector<double> values) {
    return pick(values, dice());
}

uint32_t Probabilities::pick() {
    return pick(dice());
}

bool Probabilities::load(const std::string &name) {
    string file_path =  name + _extension;
    std::ifstream file;
    file.open(file_path.c_str());
    if (!file.good()) return false;
    string line;
    while (getline(file, line)){
        istringstream ss(line);
        uint32_t p;
        ss >> p;
        _chances.push_back(p);
    }
    return true;
}

bool Probabilities::save(const std::string &name) const {
    string file_path = name + _extension;
    std::ofstream file;
    file.open(file_path.c_str());
    if (!file.good()) return false;
    for (auto p : _chances){
        file
                << p ;
        file << std::endl;
    }
    return true;
}

Probabilities &Probabilities::operator=(const Probabilities &p) {
    if (this != &p){
        _chances.clear();
        for (auto c:p._chances) _chances.push_back(c);
    }
    return *this;
}

uint32_t Probabilities::dice(uint32_t max) {
    return rand() % max;
}
