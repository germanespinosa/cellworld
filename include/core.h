#pragma once
#include <ge211.h>
#include <iostream>
#include <cinttypes>
#include <string>
#include <vector>

/*#ifdef DEBUG
    #define D(X) X
    #define L(X) std::cout << X << std::endl;
    #define E(X) std::cerr << X << std::endl;
#else*/
    #define D(X) ;
    #define L(X) ;
    #define E(X) ;
//#endif

namespace cell_world{
    const int16_t Not_found = -1;
    enum Cell_type {
        Circle,
        Square
    };
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
    enum Icon{
        No_icon,
        Arrow_icon,
        Green_arrow_icon,
        Prey_icon,
        Predator_icon,
        Bridge_icon,
        Custom_icon_0 = 100,
        Custom_icon_1,
        Custom_icon_2,
        Custom_icon_3,
        Custom_icon_4,
        Custom_icon_5,
        Custom_icon_6,
        Custom_icon_7,
        Custom_icon_8,
        Custom_icon_9,
        Custom_icon_10,
        Custom_icon_11,
        Custom_icon_12,
        Custom_icon_13,
        Custom_icon_14,
        Custom_icon_15,
        Custom_icon_16,
        Custom_icon_17,
        Custom_icon_18,
        Custom_icon_19,
        Custom_icon_20,
        Custom_icon_21,
        Custom_icon_22,
        Custom_icon_23,
        Custom_icon_24,
        Custom_icon_25,
        Custom_icon_26,
        Custom_icon_27,
        Custom_icon_28,
        Custom_icon_29,
        Custom_icon_30
    };
    struct Coordinates{
        int8_t x,y;
        bool is_origin() const;
        int rotation() const;
        bool operator ==(const Coordinates &) const;
        bool operator !=(const Coordinates &) const;
        Coordinates &operator =(const std::string &);
        Coordinates operator +=(const Coordinates &);
        Coordinates operator +(const Coordinates &) const;
        Coordinates operator -(const Coordinates &) const;
        Coordinates operator -() const;
        friend std::ostream& operator<<(std::ostream& , const Coordinates& );
    };

    using Move = Coordinates;

    struct Location{
        double x,y;
        bool operator ==(const Location &) const;
        bool operator !=(const Location &) const;
        Location operator +=(const Location &);
        Location operator +(const Location &) const;
        Location operator -(const Location &) const;
        Location operator -() const;
        Location operator *(double)const;
        double mod() const;
        double dist(const Location &) const;
        double dist(const Location &, const Location &) const;
        friend std::ostream& operator<<(std::ostream& , const Location& );
    };

    struct Cell{
        Cell();
        Cell(const cell_world::Cell&) = default;
        Cell(Cell_type, Coordinates, Location, double , bool);
        Cell_type cell_type;
        uint32_t id;
        Coordinates coordinates;
        Location location;
        double value;
        bool occluded;
        Icon icon;
        Coordinates direction;
        bool operator == (const Cell&) const;
        Cell &operator = (const Cell&);
        friend std::ostream& operator<<(std::ostream& , const Cell& );
    };
    std::vector<int> histogram(std::vector<int>);
    double entropy(const std::vector<int>&);
    std::vector<uint32_t> new_index(uint32_t);
    std::vector<uint32_t> new_index(std::vector<double>, bool);
    double max(const std::vector<double> &);
    uint32_t sum(const std::vector<uint32_t>& );
}