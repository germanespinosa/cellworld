#pragma once
#include <iostream>
#include <cinttypes>
#include <string>
#include <vector>
#include <json_cpp.h>

#ifdef DEBUG
#else
#endif

#define SIMILAR(A,B,M) ((A-M < B) && (A+M > B))

namespace cell_world{
    const int Not_found = -1;

    struct Cell_descriptor : json_cpp::Json_object{
        Cell_descriptor ();
        Cell_descriptor (int, double, double);
        int sides;
        double radius;
        double rotation;
        double theta() const;
        Json_object_members({
                                Add_member(sides);
                                Add_member(radius);
                                Add_member(rotation);
                            })
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
        Location operator/(double) const;
        double mod() const;
        double atan(const Location &) const;
        double dist(const Location &) const;
        double dist(const Location &, const Location &) const;
        double manhattan(const Location &) const;
        Location move(double theta, double dist) const;
        Json_object_members({
                         Add_member(x);
                         Add_member(y);
        })
    };

    using Location_list = json_cpp::Json_vector<Location>;

    struct Cell : json_cpp::Json_object{
        Cell();
        Cell(const cell_world::Cell&) = default;
        Cell(Coordinates, Location, bool);
        Cell(unsigned int, Coordinates, Location, bool);
        unsigned int id;
        Coordinates coordinates;
        Location location;
        bool occluded;
        static Cell &ghost_cell();
        bool operator != (const Cell&) const;
        bool operator == (const Cell&) const;
        Cell &operator = (const Cell&);
        operator bool () const{
            return this->id != ghost_cell().id;
        };
        Json_object_members({
            Add_member(id);
            Add_member(coordinates);
            Add_member(location);
            Add_member(occluded);
            Ignore_member("cell_type");
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