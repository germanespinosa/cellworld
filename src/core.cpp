#include <core.h>
#include <ge211.h>
#include <iostream>

using namespace std;
using namespace cell_world;
using namespace ge211;

bool Coordinates::operator ==(const Coordinates c) const {
    return c.x==x && c.y == y;
}

bool Coordinates::operator !=(const Coordinates c) const {
    return c.x!=x || c.y != y;
}

Coordinates Coordinates::operator +=(const Coordinates c) {
    return { (int8_t)(x += c.x), (int8_t)(y += c.y) };
}

Coordinates Coordinates::operator +(const Coordinates c) const{
    return { (int8_t)(c.x + x), (int8_t)(c.y + y) };
}

Coordinates Coordinates::operator -(const Coordinates c) const{
    return { (int8_t)(x - c.x), (int8_t)(y - c.y) };
}

Coordinates Coordinates::operator -() const{
    return { (int8_t)(-x), (int8_t)(-y) };
}

bool Cell::operator == (const Cell& c) const {
    return (id == c.id &&
            coordinates == c.coordinates &&
            location == c.location &&
            occluded == c.occluded);
}


Cell::Cell (uint32_t id, Coordinates coordinates, Basic_position<double> location, double value, bool occluded)
{
    this->id = id;
    this->location = location;
    this->coordinates = coordinates;
    this->value = value;
    this->occluded=occluded;
}

Cell::Cell(){
    this->id = 0;
    this->location = {0,0};
    this->coordinates = {0,0};
    this->occluded= false;
    this->value=0;
}

double cell_world::entropy(std::vector<int> histogram) {
    vector<double> prob;
    int c = 0;
    for (int h:histogram) c+=h;
    for (int h:histogram) if ( h > 0 ) prob.push_back((double)h/(double)c);
    double ent=0;
    for (double p:prob) ent += p * log2(p);
    return -ent;
}

vector<int> cell_world::histogram(vector<int> values) {
    if (values.size() == 0) {
        vector<int> hist;
        return move(hist);
    } else {
        int min = values[0];
        int max = values[0];
        for (int v:values) if ( min > v ) min = v; else if ( max < v ) max = v;
        vector<int> hist(max - min + 1 );
        for (int v:values) hist[v-min]++;
        return move(hist);
    }
}

uint32_t Probabilities::size() {
    return _chances.size();
}

double Probabilities::probability(uint32_t index) {
    uint32_t chances = (*this)[index];
    return (double)chances / (double)_max();
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
    uint32_t chance_dice = (dice % _max()) + 1;
    for (action = 0 ; chance_dice > _chances[action] && action < values.size()-1; action++);
    double reward = values[indexes[action]]; // this is the expected value of the state-action pair

    uint32_t repetitions = 0;
    for (uint32_t i = 0; i < values.size(); i++) if (values[i] == reward) repetitions ++; //find how many times the same value appears

    uint32_t selection = dice % repetitions; //use the dice to determine which repetition to use
    uint32_t pick;
    for (pick = 0; values[pick] != reward || selection--; pick++); // find the correct repetition

    return pick; //pick the correct one
}

uint32_t Probabilities::_max() {
    return _chances[_chances.size()-1];
}

double Probabilities::compute(std::vector<double>values) {
    double r = 0;
    for (uint32_t i = 0; i < values.size() && i < _chances.size();i++ ) r+=values[i] * probability(i);
    return r;
}
