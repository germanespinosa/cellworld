#include <core.h>
#include <ge211.h>

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

