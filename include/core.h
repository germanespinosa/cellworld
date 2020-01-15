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
        
        bool operator ==(const Coordinates &) const;
        bool operator !=(const Coordinates &) const;
        Coordinates operator +=(const Coordinates &);
        Coordinates operator +(const Coordinates &) const;
        Coordinates operator -(const Coordinates &) const;
        Coordinates operator -() const;
    };

    struct Location{
        double x,y;
        bool operator ==(const Location &) const;
        bool operator !=(const Location &) const;
        Location operator +=(const Location &);
        Location operator +(const Location &) const;
        Location operator -(const Location &) const;
        Location operator -() const;
        Location operator *(double)const;
        double mod();
        double dist(const Location &) const;
    };

    struct Cell{
        Cell();
        Cell(uint32_t, Coordinates, Location, double , bool);
        uint32_t id;
        Coordinates coordinates{};
        Location location{0,0};
        double value{};
        bool occluded;
        bool operator == (const Cell&) const;
    };

    std::vector<int> histogram(std::vector<int>);
    double entropy(std::vector<int>);

} 