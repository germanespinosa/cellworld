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

    struct Cell{
        Cell();
        Cell(uint32_t, Coordinates, ge211::Basic_position<double>, double , bool);
        uint32_t id;
        Coordinates coordinates{};
        ge211::Basic_position<double> location{0,0};
        double value{};
        bool occluded;
        bool operator == (const Cell&) const;
    };

} 