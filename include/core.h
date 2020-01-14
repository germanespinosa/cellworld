#pragma once
#include <ge211.h>

namespace cell_world{
    const int16_t Not_found = -1;

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
    struct Coordinates{
        int8_t x,y;
        
        bool operator ==(Coordinates) const;
        bool operator !=(Coordinates) const;
        Coordinates operator +=(Coordinates);
        Coordinates operator +(Coordinates) const;
        Coordinates operator -(Coordinates) const;
        Coordinates operator -() const;
    };

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

    struct Probabilities {
        explicit Probabilities(const std::vector<uint32_t>&);
        explicit Probabilities(const std::vector<double>&);
        uint32_t size();
        double probability(uint32_t);
        uint32_t operator [](uint32_t);
        uint32_t pick (std::vector<double>, uint32_t);
        double compute(std::vector<double>);
    private:
        std::vector<uint32_t> _chances;
        uint32_t _max();
    };

    std::vector<int> histogram(std::vector<int>);
    double entropy(std::vector<int>);

} 