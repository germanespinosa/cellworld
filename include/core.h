#pragma once
#include <ge211.h>

namespace cell_world{
    const int16_t Not_found = -1;

    enum Color{
        Black,
        White,
        Red,
        Lime,
        Blue,
        Yellow,
        Cyan,
        Magenta,
        Silver,
        Gray,
        Maroon,
        Olive,
        Green,
        Purple,
        Teal,
        Navy
    };
    struct Coordinates{
        int8_t x,y;
        
        bool operator ==(Coordinates) const;
        bool operator !=(Coordinates) const;
        Coordinates operator +=(Coordinates);
        Coordinates operator +(Coordinates) const;
        Coordinates operator -(Coordinates) const;
        Coordinates operator -() const;
    };
} 