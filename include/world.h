#pragma once
#include <core.h>
#include <ge211.h>
#include "connection.h"

namespace cellworld{

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

    struct World{
        explicit World(std::string);
        bool add(Cell);
        bool load(std::string);
        bool load();
        bool save() const;
        bool save(std::string) const;
        double distance(uint32_t, uint32_t) const;
        double distance(const Cell&, const Cell&) const;
        double distance(const Cell&, const Cell&, const Cell&) const;
        uint32_t size() const;
        int32_t find(const Coordinates&) const;
        const Cell &operator[](const uint32_t& ) const;
        const Cell &operator[](const Coordinates&) const;
        void set_occlusion(uint32_t, bool);
        void set_value(uint32_t, double);
        void get_connections(Connections &, const std::vector<Coordinates> &) const;
        std::string name;
    private:
        std::vector<Cell> cells;
        std::vector<std::vector<double>> _distances;
        std::string _file_name;
        int32_t _map[256][256];
    };
}