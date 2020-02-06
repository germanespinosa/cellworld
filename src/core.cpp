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

std::string Coordinates::operator!() const {
    std::stringstream fmt;
    fmt <<"(" << (int32_t)x << "," << (int32_t)y <<")";
    return fmt.str();
}

bool Coordinates::is_origin() const {
    return x==0 && y==0;
}

int Coordinates::rotation() const {
    return atan2(x,-y) / 6.28 * 360;
}

bool Cell::operator == (const Cell& c) const {
    return id == c.id;
}

Cell::Cell (uint32_t id, Coordinates coordinates, Location location, double value, bool occluded)
{
    this->id = id;
    this->location = location;
    this->coordinates = coordinates;
    this->value = value;
    this->occluded = occluded;
    icon = No_icon;
    direction = {0,0};
}

Cell::Cell(){
    id = 0;
    location = {0,0};
    coordinates = {0,0};
    occluded = false;
    value = 0;
    icon = No_icon;
    direction = {0,0};
}

Cell &Cell::operator=(const Cell &c) {
    id = c.id;
    location = c.location;
    coordinates = c.coordinates;
    occluded = c.occluded;
    value =c.value;
    return *this;
}

double cell_world::entropy(const std::vector<int>& histogram) {
    vector<double> prob;
    int c = 0;
    for (int h:histogram) c+=h;
    for (int h:histogram) if ( h > 0 ) prob.push_back((double)h/(double)c);
    double ent=0;
    for (double p:prob) ent += p * log2(p);
    return -ent;
}

vector<int> cell_world::histogram(vector<int> values) {
    if (values.empty()) {
        vector<int> hist;
        return hist;
    } else {
        int min = values[0];
        int max = values[0];
        for (int v:values) if ( min > v ) min = v; else if ( max < v ) max = v;
        vector<int> hist(max - min + 1 );
        for (int v:values) hist[v-min]++;
        return hist;
    }
}

vector<uint32_t> cell_world::new_index(uint32_t n) {
    std::vector<uint32_t> index(n);
    for(uint32_t i=0;i<n;i++) index[i]=i;
    return index;
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

std::string Location::operator!() const {
    std::stringstream fmt;
    fmt <<"(" << x << "," << y <<")";
    return fmt.str();
}

double Location::dist(const Location &l1, const Location &l2) const {
    return abs((l2.y-l1.y) * x - (l2.x - l1.x) * y + l2.x * l1.y - l2.y * l1.x) / sqrt(pow(l2.y-l1.y,2)+pow(l2.x-l1.x,2));
}