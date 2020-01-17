#pragma once
#include <ge211.h>

namespace cell_world{
    struct Probabilities {
        Probabilities() = default;
        explicit Probabilities(const std::vector<uint32_t>&);
        explicit Probabilities(const std::vector<double>&);
        uint32_t size();
        double probability(uint32_t);
        uint32_t operator [](uint32_t);
        uint32_t pick ( std::vector<double>, uint32_t );
        uint32_t pick ( std::vector<double> );
        uint32_t pick ( uint32_t );
        uint32_t pick ();
        double compute(std::vector<double>);
        uint32_t dice();
        uint32_t max();
        bool load(const std::string&);
        bool save(const std::string&) const;
        Probabilities &operator =(const Probabilities &);
    private:
        std::vector<uint32_t> _chances;
        const std::string _extension = ".pattern";
        double _get_probability(uint32_t, uint32_t);
    };

}