#pragma once
#include <ge211.h>
#include <iostream>
#include <cinttypes>
#include <string>
#include <vector>
#include <json_paser.h>

#ifdef DEBUG
#else
#endif

namespace cell_world{
    const int Not_found = -1;
    enum Cell_type : int{
        Circle,
        Square
    };
    enum Color : int {
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
    enum Icon : int {
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
    struct Coordinates : Json_object{
        Coordinates ();
        Coordinates (int x, int y);
        int x{},y{};
        bool is_origin() const;
        int rotation() const;
        bool operator ==(const Coordinates &) const;
        bool operator !=(const Coordinates &) const;
        Coordinates &operator =(const std::string &);
        Coordinates operator +=(const Coordinates &);
        Coordinates operator +(const Coordinates &) const;
        Coordinates operator -(const Coordinates &) const;
        Coordinates operator -() const;
        unsigned int manhattan(const Coordinates &) const;
        void json_set_parser(Json_parser &) override;
    };

    struct Coordinates_list : Json_parsable_vector<Coordinates>{
    };

    using Move = Coordinates;

    struct Location : Json_object {
        Location();
        Location(double x, double y);
        double x{}, y{};
        bool operator==(const Location &) const;
        bool operator!=(const Location &) const;
        Location operator+=(const Location &);
        Location operator+(const Location &) const;
        Location operator-(const Location &) const;
        Location operator-() const;
        Location operator*(double) const;
        double mod() const;
        double dist(const Location &) const;
        double dist(const Location &, const Location &) const;
        double manhattan(const Location &) const;
        void json_set_parser(Json_parser &) override;
    };

    struct Cell : Json_object{
        Cell();
        Cell(const cell_world::Cell&) = default;
        Cell(Cell_type, Coordinates, Location, double , bool);
        Cell_type cell_type;
        unsigned int id;
        Coordinates coordinates;
        Location location;
        double value;
        bool occluded;
        Icon icon;
        Coordinates direction;
        bool operator == (const Cell&) const;
        Cell &operator = (const Cell&);
        void json_set_parser(Json_parser &) override;
    };

    struct Cell_list : Json_parsable_vector<Cell> {
    };

    std::vector<int> histogram(std::vector<int>);
    double entropy(const std::vector<int>&);
    std::vector<unsigned int> new_index(unsigned int);
    std::vector<unsigned int> new_index(std::vector<double>, bool);
    double max(const std::vector<double> &);
    unsigned int sum(const std::vector<unsigned int>& );
}