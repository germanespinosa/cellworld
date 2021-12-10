#include <cell_world/core.h>
#include <algorithm>
#include <cmath>
#include <sys/stat.h>
#include <filesystem>

using namespace std;

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

    bool Cell::operator!=(const Cell &c) const {
        return id != c.id;
    }

    Cell::Cell():
            Cell({0,0}, {0,0},false){
    }

    Cell &Cell::operator=(const Cell &c) {
        id = c.id;
        location = c.location;
        coordinates = c.coordinates;
        occluded = c.occluded;
        return *this;
    }

    Cell &Cell::ghost_cell() {
        static Cell c ({100000,100000}, {100000,100000}, true);
        c.id = 100000;
        return c;
    }

    Cell::Cell(Coordinates coordinates, Location location, bool occluded) :
            id(0),
            coordinates(coordinates),
            location(location),
            occluded(occluded) { }

    Cell::Cell(const Coordinates &coordinates):
    Cell(coordinates,{0,0},false){
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

    Location Location::operator/(double d) const {
        return {x/d, y/d};
    }

    double Location::mod() const {
        return sqrt(y*y + x*x);
    }

    double Location::dist(const Location &l) const {
        return (*this - l).mod();
    }

    double Location::dist(const Location &line_start, const Location &line_end) const {
        double normal_length = line_end.dist(line_start);
        double distance = (double)((x - line_start.x) * (line_end.y - line_start.y) - (y - line_start.y) * (line_end.x - line_start.x)) / normal_length;
        return abs(distance);
    }

    double Location::dist() const {
        return sqrt(x * x + y * y);
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
        x(x),y(y){;
    }

    Coordinates::Coordinates() = default;

    Location::Location(double x, double y) :
        x(x), y(y){
    }

    Location::Location() = default;

    double Location::atan(const Location &l) const {
        auto v=l-*this;
        return v.atan();
    }

    Location Location::move(double theta, double dist) const{
        return {x + sin(theta) * dist, y + cos(theta) * dist};
    }

    void Location::transform(const Transformation &transformation) {
        auto theta = to_radians(transformation.rotation);
        x = x + sin(theta) * transformation.size;
        y = y + cos(theta) * transformation.size;
    }

    Transformation Location::transformation() const {
        return Transformation(dist(), to_degrees(atan()));
    }

    double Location::atan() const {
        return atan2(x,y);
    }

    Transformation Location::transformation(const Location &l) const {
        auto d = l - *this;
        return d.transformation();
    }

    double angle_difference(double a1, double a2) {
        a1 = normalize(a1);
        a2 = normalize(a2);
        if (a1 > a2) {
            auto d = a1 - a2;
            if (d < M_PI) return d;
            else return a2 + M_PI * 2.0 - a1;
        } else {
            auto d = a2 - a1;
            if (d < M_PI) return d;
            else return a1 + M_PI * 2.0 - a2;
        }
    }

    int direction(double a1, double a2) {
        a1 = normalize(a1);
        a2 = normalize(a2);
        if (a1 > a2) {
            auto d = a1 - a2;
            if (d < M_PI) return 1;
            else return -1;
        } else {
            auto d = a2 - a1;
            if (d < M_PI) return -1;
            else return 1;
        }
    }

    double normalize(double angle) {
        while (angle < 0) angle += 2.0 * M_PI;
        while (angle > 2 * M_PI) angle -= 2.0 * M_PI;
        return angle;
    }

    double to_radians(double degrees) {
        return degrees / 360.0 * 2.0 * M_PI;
    }

    double to_degrees(double radians) {
        return radians * 360.0 / (2.0 * M_PI);
    }

    double normalize_degrees(double angle) {
        while (angle < -180.0) angle += 360.0;
        while (angle > 180.0) angle -= 360.0;
        return angle;
    }

    bool file_exists(const string &file_path){
        struct stat buffer;
        return (stat (file_path.c_str(), &buffer) == 0);
    }

    bool file_exists(const std::vector<std::string> &file_paths){
        for (auto &file_path : file_paths) {
            if (!file_exists(file_path)) return false;
        }
        return true;
    }

    bool file_exists(const std::string &path, const std::vector<std::string> &file_names){
        string fixed_path = path;
        if (!fixed_path.ends_with('/')) fixed_path += '/';
        for (auto &file_name : file_names) {
            if (!file_exists(fixed_path + file_name)) return false;
        }
        return true;
    }

    bool folder_exists(const string &path){
        return filesystem::exists(path);
    }

    bool create_folder(const string &path){
        if (filesystem::exists(path)) return false;
        filesystem::create_directory(path);
        return true;
    }

    bool angle_between(double v, double l1, double l2, bool inclusive){
        if (inclusive) {
            if (angle_difference(v,l1)==0 || angle_difference(v,l2)==0) return true;
        }
        return (direction(v,l1) + direction(v,l2)) == 0;
    }

    double Transformation::theta() const {
        return to_radians(rotation);
    }


    cell_world::Transformation::Transformation() = default;

    cell_world::Transformation::Transformation(double size, double rotation):
            size(size), rotation(rotation){

    }
}

