#include <core.h>
#include <ge211.h>
#include <iostream>
#include <algorithm>
#include <json.h>

using namespace std;
using namespace ge211;

namespace cell_world {

    bool Coordinates::operator ==(const Coordinates &c) const {
        return c.x==x && c.y == y;
    }

    bool Coordinates::operator !=(const Coordinates &c) const {
        return !(*this==c);
    }

    Coordinates Coordinates::operator +=(const Coordinates &c) {
        return { (x += c.x), (y += c.y) };
    }

    Coordinates Coordinates::operator +(const Coordinates &c) const{
        return { (c.x + x), (c.y + y) };
    }

    Coordinates Coordinates::operator -(const Coordinates &c) const{
        return { (x - c.x), (y - c.y) };
    }

    Coordinates Coordinates::operator -() const{
        return { (-x), (-y) };
    }

    bool Coordinates::is_origin() const {
        return x==0 && y==0;
    }

    int Coordinates::rotation() const {
        return (int)(atan2(x,-y) / 6.28 * 360.0);
    }

    bool Cell::operator == (const Cell& c) const {
        return id == c.id;
    }

    Cell::Cell (Cell_type cell_type, Coordinates coordinates, Location location, double value, bool occluded):
            cell_type (cell_type),
            id(0),
            coordinates(coordinates),
            location(location),
            value(value),
            occluded(occluded),
            icon(No_icon),
            direction({0,0})
            { Cell();}

    Cell::Cell(){
    }

    void Cell::json_set_parser(Json_parser &p) {
        p.json_add_member("id", true, id);
        p.json_add_member("type", true, (int &)cell_type);
        p.json_add_member("coordinates", true, coordinates);
        p.json_add_member("location", true, location);
        p.json_add_member("occluded", true, occluded);
        p.json_add_member("value", false, value);
        p.json_add_member("icon", false, (int &)icon);
        p.json_add_member("direction",false,direction);
    }


    Cell &Cell::operator=(const Cell &c) {
        cell_type = c.cell_type;
        id = c.id;
        location = c.location;
        coordinates = c.coordinates;
        occluded = c.occluded;
        value =c.value;
        return *this;
    }

    double entropy(const std::vector<int>& histogram) {
        vector<double> prob;
        int c = 0;
        for (int h:histogram) c+=h;
        for (int h:histogram) if ( h > 0 ) prob.push_back((double)h/(double)c);
        double ent=0;
        for (double p:prob) ent += p * log2(p);
        return -ent;
    }

    vector<int> histogram(vector<int> values) {
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

    vector<unsigned int> new_index(unsigned int n) {
        std::vector<unsigned int> index(n);
        for(unsigned int i=0;i<n;i++) index[i]=i;
        return index;
    }

    vector<unsigned int> new_index(vector<double>values, bool ascending = true){
        auto index = new_index(values.size()); // creates an index vector for the options
        for (unsigned int i = 1; i < index.size(); i++) // sort the indexes of the options by expected reward descending
            for (unsigned int j = i; j > 0 && (ascending?values[index[j - 1]] > values[index[j]]:values[index[j - 1]] < values[index[j]]); j--)
                swap(index[j - 1], index[j]);
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

    double Location::mod() const {
        return sqrt(y*y + x*x);
    }

    double Location::dist(const Location &l) const {
        return (*this - l).mod();
    }

    double Location::dist(const Location &l1, const Location &l2) const {
        return abs((l2.y-l1.y) * x - (l2.x - l1.x) * y + l2.x * l1.y - l2.y * l1.x) / sqrt(pow(l2.y-l1.y,2)+pow(l2.x-l1.x,2));
    }

    Coordinates &Coordinates::operator=(const string &str) {
        string s;
        for (auto c : str) if ((c>='0' && c<='9') || c==',' || c=='-') s += c;
        stringstream s_stream(s);
        if (s_stream.good()) {
            string substr;
            getline(s_stream, substr, ','); //get first string delimited by comma
            x = stoi(substr);
            if (s_stream.good()) {
                getline(s_stream, substr, ','); //get first string delimited by comma
                y = stoi(substr);
            }
        }
        return *this;
    }

    unsigned int Coordinates::manhattan(const Coordinates &c) const {
        return abs(c.x-x) + abs(c.y-y);
    }

    double Location::manhattan(const Location &l) const {
        return abs(l.x-x) + abs(l.y-y);
    }

    double max(const std::vector<double> &values){
        double m = values[0];
        for (auto &v:values) m = v>m?v:m;
        return m;
    }
    unsigned int sum(const std::vector<unsigned int> &values){
        unsigned int s = 0;
        for (auto &v:values) s+=v;
        return s;
    }

    Coordinates::Coordinates(int x, int y):
        x(x),y(y){
        Coordinates();
    }

    Coordinates::Coordinates() {
    }

    void Coordinates::json_set_parser(Json_parser &p) {
        p.json_add_member("x",true,x);
        p.json_add_member("y",true,y);
    }

    Location::Location(double x, double y) :
        x(x), y(y){
        Location();
    }

    Location::Location() {
    }

    void Location::json_set_parser(Json_parser &p) {
        p.json_add_member("x",true,x);
        p.json_add_member("y",true,y);
    }

}