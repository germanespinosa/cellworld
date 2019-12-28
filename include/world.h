#pragma once
#include <core.h>
#include <ge211.h>
namespace cellworld{

    struct Cell{
        Cell();
        Cell(uint32_t, Coordinates, ge211::Basic_position<double>, double , bool);
        uint32_t id;
        Coordinates coordinates;
        ge211::Basic_position<double> location{0,0};
        double value;
        bool occluded;
        bool operator == (const Cell&) const;
    };
    
    struct World{
        bool add(Cell);
        bool load(const std::string);
        bool load();
        bool save() const ;
        bool save(const std::string) const ;
        double distance(const uint32_t, const uint32_t)  const;
        double distance(const Cell&, const Cell&)  const;
        double distance(const Cell&, const Cell&, const Cell&)  const;
        uint32_t size() const;
        int32_t find(const Coordinates&) const;
        const Cell &operator[](const uint32_t& );
        const Cell &operator[](const Coordinates&);
        void set_occlusion(uint32_t, bool);
        void set_value(uint32_t, double);
    private:
        std::vector<Cell> cells;
        std::vector<std::vector<double>> _distances;
        std::string _file_name;
    };
}  