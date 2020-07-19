#pragma once
#include <iostream>
#include <cinttypes>
#include <string>
#include <vector>
#include <json_cpp.h>

#ifdef DEBUG
#else
#endif

namespace cell_world{
    const int Not_found = -1;
    enum Cell_type : int {
        Circle,
        Square
    };
    struct Coordinates : json_cpp::Json_object{
        Coordinates ();
        Coordinates (int x, int y);
        int x{},y{};
        bool is_origin() const;
        int rotation() const;
        bool operator ==(const Coordinates &) const;
        bool operator !=(const Coordinates &) const;
        Coordinates operator +=(const Coordinates &);
        Coordinates operator +(const Coordinates &) const;
        Coordinates operator -(const Coordinates &) const;
        Coordinates operator -() const;
        unsigned int manhattan(const Coordinates &) const;
        Json_set_builder({
            Json_add_member(x,true);
            Json_add_member(y,true);
        })
    };

    using Coordinates_list = json_cpp::Json_vector<Coordinates>;

    using Move = Coordinates;

    using Move_list = json_cpp::Json_vector<Move>;

    struct Location : json_cpp::Json_object {
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
        Json_set_builder({
            Json_add_member(x,true);
            Json_add_member(y,true);
        })
    };

    using Location_list = json_cpp::Json_vector<Location>;

    struct Cell : json_cpp::Json_object{
        Cell();
        Cell(const cell_world::Cell&) = default;
        Cell(Cell_type, Coordinates, Location, double , bool);
        Cell_type cell_type;
        unsigned int id;
        Coordinates coordinates;
        Location location;
        double value;
        bool occluded;
        bool operator == (const Cell&) const;
        Cell &operator = (const Cell&);
        Json_set_builder({
            Json_add_member(id, true);
            Json_add_member(cell_type, true);
            Json_add_member(coordinates, true);
            Json_add_member(location, true);
            Json_add_member(occluded, true);
            Json_add_member(value, false);
        })
    };

    using Cell_list = json_cpp::Json_vector<Cell>;

    std::vector<int> histogram(std::vector<int>);
    double entropy(const std::vector<int>&);
    std::vector<unsigned int> new_index(unsigned int);
    std::vector<unsigned int> new_index(std::vector<double>, bool);
    double max(const std::vector<double> &);
    unsigned int sum(const std::vector<unsigned int>& );
}