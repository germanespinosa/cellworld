#include <cell_world/core.h>
#include <algorithm>
#include <cmath>
#include <sys/stat.h>
#include <filesystem>
#include <stdlib.h>
#include <cell_world/cell.h>

using namespace std;

namespace cell_world {
    bool Cell::operator == (const Cell& c) const {
        return id == c.id;
    }

    bool Cell::operator!=(const Cell &c) const {
        return id != c.id;
    }

    Cell::Cell():
            Cell({0,0}, {0,0},false){
    }

    Cell &Cell::operator=(const Cell &c) {
        id = c.id;
        location = c.location;
        coordinates = c.coordinates;
        occluded = c.occluded;
        return *this;
    }

    Cell &Cell::ghost_cell() {
        static Cell c ({100000,100000}, {100000,100000}, true);
        c.id = 100000;
        return c;
    }

    Cell::Cell(Coordinates coordinates, Location location, bool occluded) :
            id(0),
            coordinates(coordinates),
            location(location),
            occluded(occluded) { }

    Cell::Cell(const Coordinates &coordinates):
            Cell(coordinates,{0,0},false){
    }
}