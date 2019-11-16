#include <core.h>
#include <ge211.h>

using namespace std;
using namespace cellworld;


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
