#pragma once
#include <ge211.h>

namespace cell_world{
    struct Chance {
        Chance() = default;
        explicit Chance(const std::vector<uint32_t>&);
        explicit Chance(const std::vector<double>&);
        uint32_t size();
        double probability(uint32_t);
        uint32_t operator [](uint32_t);
        uint32_t pick ( std::vector<double> &, uint32_t );
        uint32_t pick ( std::vector<double> &);
        uint32_t pick ( uint32_t );
        uint32_t pick ();
        double compute(std::vector<double> &);
        uint32_t max();
        bool load(const std::string&);
        bool save(const std::string&) const;
        Chance &operator =(const Chance &);
        static uint32_t dice();
        static uint32_t dice(uint32_t);
        static int32_t dice(int32_t, int32_t);
        static double dice_double (double);
        static double dice_double (double,double);

        Chance operator!();
    private:
        std::vector<uint32_t> _chances;
        const std::string _extension = ".pattern";
        double _get_probability(uint32_t, uint32_t);
    };

}