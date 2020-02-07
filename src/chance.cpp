#include <chance.h>
#include <core.h>
#include <fstream>
#include <iostream>
#include <utility>

using namespace cell_world;
using namespace std;

uint32_t Chance::size() {
    return _chances.size();
}

double Chance::probability(uint32_t index) {
    return _get_probability(index,max());
}

uint32_t Chance::operator[](uint32_t index) {
    if (index==0) return _chances[index];
    return _chances[index] - _chances[index-1];
}

Chance::Chance(const std::vector<uint32_t>& individual_chances) {
    uint32_t accumulated = 0;
    for ( uint32_t ic:individual_chances ){
        accumulated += ic;
        _chances.push_back(accumulated);
    }
    if (accumulated==0) for (uint32_t i=0 ;i<_chances.size();i++) _chances[i]=i+1;
}

Chance::Chance(const std::vector<double>&individual_probabilities) {
    double accumulated = 0;
    for ( double ic:individual_probabilities ){
        _chances.push_back( ( ic + accumulated ) * 10000 );
        accumulated += ic;
    }
}

uint32_t Chance::pick(std::vector<double> &values, uint32_t dice) {
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

uint32_t Chance::max() {
    return _chances[_chances.size()-1];
}

double Chance::compute(std::vector<double> &values) {
    double r = 0;
    for (uint32_t i = 0; i < values.size() && i < _chances.size();i++ ) r+=values[i] * _get_probability(i,_chances[values.size()-1]);
    return r;
}

double Chance::_get_probability(uint32_t index, uint32_t max) {
    uint32_t chances = (*this)[index];
    return (double)chances / (double)max;
}

uint32_t Chance::pick(uint32_t dice) {
    uint32_t pick;
    uint32_t chance_dice = (dice % max()) + 1;
    for (pick = 0 ; chance_dice > _chances[pick] && pick < _chances.size()-1; pick++);
    return pick;
}

uint32_t Chance::pick(std::vector<double> &values) {
    return pick(values, dice());
}

uint32_t Chance::pick() {
    return pick(dice());
}

bool Chance::load(const std::string &name) {
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

bool Chance::save(const std::string &name) const {
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

Chance &Chance::operator=(const Chance &p) {
    if (this != &p){
        _chances.clear();
        for (auto c:p._chances) _chances.push_back(c);
    }
    return *this;
}

uint32_t Chance::dice(uint32_t max) {
    return dice() % max;
}

Chance Chance::operator!() {
    if (_chances.empty()) return Chance();
    vector<uint32_t> individual_chances;
    uint32_t acum = 0;
    uint32_t min = _chances[_chances.size()-1];
    uint32_t max = 0;
    for(auto &c:_chances) {
        uint32_t iv=c-acum;
        if (iv < min) min = iv;
        if (iv > max) max = iv;
        individual_chances.push_back(iv);
        acum = c;
    }
    uint32_t target = min + max;
    for ( uint32_t &ic:individual_chances ){
        ic = target - ic;
    }
    return Chance(individual_chances);
}

int32_t Chance::dice(int32_t min, int32_t max) {
    return (dice() % (max-min+1)) + min;
}

uint32_t Chance::dice() {
    return rand();
}

double Chance::dice_double(double max) {
    return (double)dice() * max / RAND_MAX;
}

double Chance::dice_double(double min,double max) {
    return (double)dice() * (max-min) / RAND_MAX + min;
}

Chance::Chance(double first_option, uint32_t option_count){
    double accumulated = first_option;
    _chances.push_back( first_option * 10000 );
    for (uint32_t i=1;i<option_count;i++){
        double ic = (1-first_option)/(double)(option_count-1);
        _chances.push_back( ( ic + accumulated ) * 10000 );
        accumulated += ic;
    }
}

Chance::Chance(uint32_t c){
    for (uint32_t i=0;i<c;i++) _chances.push_back(i+1);
}