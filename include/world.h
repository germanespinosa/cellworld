#pragma once
#include <core.h>
#include <ge211.h>
namespace cellworld{

    struct Cell{
        Cell();
        Cell(uint32_t, Coordinates, ge211::Basic_position<double>, std::vector<uint32_t>, double , bool);
        uint32_t id;
        Coordinates coordinates;
        ge211::Basic_position<double> location{0,0};
        std::vector<uint32_t> connections;
        double value;
        bool occluded;
        bool operator == (const Cell&) const;
    };
    
    struct World{
        bool add(const Cell);
        bool load(const std::string);
        bool save(const std::string) const ;
        double distance(const Cell&, const Cell&)  const;
        double distance(const Cell&, const Cell&, const Cell&)  const;
        uint32_t size() const;
        int32_t find(const Coordinates&) const;
        void connect(std::vector<Coordinates>);
        Cell &operator[](const uint32_t& );
        Cell &operator[](const Coordinates&);
    private:
        std::vector<Cell> cells;
    };
}  