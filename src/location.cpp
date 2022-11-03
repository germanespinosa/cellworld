#include <cell_world/location.h>
#include <cell_world/geometry.h>

namespace cell_world{
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

    Location Location::operator*(float m) const {
        return {x*m, y*m};
    }

    Location Location::operator/(float d) const {
        return {x/d, y/d};
    }

    float Location::mod() const {
        return sqrt(y*y + x*x);
    }

    float Location::dist(const Location &l) const {
        return (*this - l).mod();
    }

    float Location::dist(const Location &line_start, const Location &line_end) const {
        float normal_length = line_end.dist(line_start);
        float distance = (float)((x - line_start.x) * (line_end.y - line_start.y) - (y - line_start.y) * (line_end.x - line_start.x)) / normal_length;
        return abs(distance);
    }

    float Location::dist() const {
        return sqrt(x * x + y * y);
    }

    float Location::manhattan(const Location &l) const {
        return abs(l.x-x) + abs(l.y-y);
    }

    Location::Location(float x, float y) :
            x(x), y(y){
    }

    Location::Location() = default;

    float Location::atan(const Location &l) const {
        auto v=l-*this;
        return v.atan();
    }

    Location Location::move(float theta, float dist) const{
        return {x + (float)sin(theta) * dist, y + (float)cos(theta) * dist};
    }

    void Location::transform(const Transformation &transformation) {
        auto theta = to_radians(transformation.rotation);
        x = x + sin(theta) * transformation.size;
        y = y + cos(theta) * transformation.size;
    }

    Transformation Location::transformation() const {
        return Transformation(dist(), to_degrees(atan()));
    }

    float Location::atan() const {
        return atan2(x,y);
    }

    Transformation Location::transformation(const Location &l) const {
        auto d = l - *this;
        return d.transformation();
    }

    Location Location::operator*(const Location & other) const {
        return {x*other.x, y*other.y};
    }

    Location Location::orthogonal(const Location &line_start, const Location &line_ends) const {
        auto line_theta = line_start.atan(line_ends);
        auto orthogonal_theta = line_theta + M_PI / 2;
        auto segment_length = dist(line_start,line_ends);
        return move(orthogonal_theta, segment_length);
    }
}