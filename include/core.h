#pragma once
#include <ge211.h>
#include <iostream>
#include <cinttypes>
#include <string>
#include <vector>

#ifdef DEBUG
    #define D(X) X
    #define L(X) std::cout << X << std::endl;
    #define E(X) std::cerr << X << std::endl;
#else
    #define D(X) ;
    #define L(X) ;
    #define E(X) ;
#endif

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
        Custom_0_icon = 100,
        Custom_1_icon,
        Custom_2_icon,
        Custom_3_icon,
        Custom_4_icon,
        Custom_5_icon,
        Custom_6_icon,
        Custom_7_icon,
        Custom_8_icon,
        Custom_9_icon,
        Custom_10_icon,
        Custom_11_icon,
        Custom_12_icon,
        Custom_13_icon,
        Custom_14_icon,
        Custom_15_icon,
        Custom_16_icon,
        Custom_17_icon,
        Custom_18_icon,
        Custom_19_icon,
        Custom_20_icon,
        Custom_21_icon,
        Custom_22_icon,
        Custom_23_icon,
        Custom_24_icon,
        Custom_25_icon,
        Custom_26_icon,
        Custom_27_icon,
        Custom_28_icon,
        Custom_29_icon,
        Custom_30_icon
    };
    struct Coordinates{
        int8_t x,y;
        bool is_origin() const;
        int rotation() const;
        bool operator ==(const Coordinates &) const;
        bool operator !=(const Coordinates &) const;
        Coordinates operator +=(const Coordinates &);
        Coordinates operator +(const Coordinates &) const;
        Coordinates operator -(const Coordinates &) const;
        Coordinates operator -() const;
        friend std::ostream& operator<<(std::ostream& , const Coordinates& );
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
        std::string operator !() const;
        double mod();
        double dist(const Location &) const;
        double dist(const Location &, const Location &) const;
        friend std::ostream& operator<<(std::ostream& , const Location& );
    };

    struct Cell{
        Cell();
        Cell(Cell_type, uint32_t, Coordinates, Location, double , bool);
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