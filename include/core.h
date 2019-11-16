#pragma once
#include <ge211.h>

namespace cellworld{
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
        
        bool operator ==(const Coordinates) const;
        bool operator !=(const Coordinates) const;
        Coordinates operator +=(const Coordinates);
        Coordinates operator +(const Coordinates) const;
        Coordinates operator -(const Coordinates) const;
        Coordinates operator -() const;
    };
} 