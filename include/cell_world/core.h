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
        Json_object_members({
            Add_member(x);
            Add_member(y);
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
        double atan(const Location &) const;
        double dist(const Location &) const;
        double dist(const Location &, const Location &) const;
        double manhattan(const Location &) const;
        Json_object_members({
                         Add_member(x);
                         Add_member(y);
        })
    };

    using Location_list = json_cpp::Json_vector<Location>;

    struct Cell : json_cpp::Json_object{
        Cell();
        Cell(const cell_world::Cell&) = default;
        Cell(Cell_type, Coordinates, Location, bool);
        Cell(unsigned int, Cell_type, Coordinates, Location, bool);
        unsigned int id;
        Cell_type cell_type;
        Coordinates coordinates;
        Location location;
        bool occluded;
        static Cell &ghost_cell();
        bool operator != (const Cell&) const;
        bool operator == (const Cell&) const;
        Cell &operator = (const Cell&);
        Json_object_members({
            Add_member(id);
            Add_member(cell_type);
            Add_member(coordinates);
            Add_member(location);
            Add_member(occluded);
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