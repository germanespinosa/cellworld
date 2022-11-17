#pragma once
#include <json_cpp.h>
#include <iostream>
#include <cinttypes>
#include <string>
#include <vector>
#include <cell_world/coordinates.h>
#include <cell_world/location.h>

namespace cell_world {

    struct Cell : json_cpp::Json_object {
        Cell();

        Cell(const Cell &) = default;

        Cell(const Coordinates &);

        Cell(Coordinates, Location, bool);

        unsigned int id;
        Coordinates coordinates;
        Location location;
        bool occluded;

        static Cell &ghost_cell();

        bool operator!=(const Cell &) const;

        bool operator==(const Cell &) const;

        Cell &operator=(const Cell &);

        operator bool() const {
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
}