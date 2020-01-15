#include <core.h>
#include <ge211.h>
#include <iostream>

using namespace std;
using namespace cell_world;
using namespace ge211;

bool Coordinates::operator ==(const Coordinates &c) const {
    return c.x==x && c.y == y;
}

bool Coordinates::operator !=(const Coordinates &c) const {
    return !(*this==c);
}

Coordinates Coordinates::operator +=(const Coordinates &c) {
    return { (int8_t)(x += c.x), (int8_t)(y += c.y) };
}

Coordinates Coordinates::operator +(const Coordinates &c) const{
    return { (int8_t)(c.x + x), (int8_t)(c.y + y) };
}

Coordinates Coordinates::operator -(const Coordinates &c) const{
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


Cell::Cell (uint32_t id, Coordinates coordinates, Location location, double value, bool occluded)
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

bool Location::operator==(const Location &l) const {
    return l.x == x && l.y == y;
}

bool Location::operator!=(const Location &l) const {
    return !(*this == l);
}

Location Location::operator+=(const Location &l) {
    x+=l.x;
    y+=l.y;
    return *this;
}

Location Location::operator+(const Location &l) const {
    return {x+l.x,y+l.y};
}

Location Location::operator-(const Location &l) const {
    return {x-l.x,y-l.y};
}

Location Location::operator-() const {
    return {-x,-y};
}

Location Location::operator*(double m) const {
    return {x*m, y*m};
}

double Location::mod() {
    return sqrt(y*y + x*x);
}

double Location::dist(const Location &l) const {
    return (*this - l).mod();
}
