#pragma once
#include <cell_world/types.h>
#include <cell_world/cell.h>

namespace cell_world{

    struct Cell_group_builder: Json_unsigned_int_vector {
        static Cell_group_builder get_from_parameters_name(const std::string &, const std::string &);
        static Cell_group_builder get_from_parameters_name(const std::string &, const std::string &, const std::string &);
    };

    struct Cell_reference : json_cpp::Json_base, std::reference_wrapper<const Cell>{
        Cell_reference() : Cell_reference(Cell()) {};
        explicit Cell_reference(const Cell &);
        virtual void json_write(std::ostream &o) const;
        operator const Cell&() const;
    };

    struct Cell_group : json_cpp::Json_vector<Cell_reference>{
        Cell_group();
        Cell_group(const cell_world::Cell_group&) = default;
        bool add(const Cell &);
        bool contains(unsigned int) const;
        bool contains(const Cell &) const;
        void clear();
        int find(unsigned int) const;
        int find(const Cell &) const;
        int find(const Location &) const;
        int find(const Coordinates &) const;
        std::vector<float> get_distances(const Cell &) const;
        Cell_group random_shuffle() const;
        Cell_group occluded_cells() const;
        Cell_group free_cells() const;
        Cell_group_builder get_builder() const;
        Cell_group &operator = (const Cell_group &);
        Cell_group &operator += (const Cell_group &);
        Cell_group &operator += (const Cell &);
        Cell_group &operator -= (const Cell_group &);
        Cell_group &operator -= (const Cell &);
        Cell_group operator + (const Cell_group &) const;
        Cell_group operator + (const Cell &) const;
        Cell_group operator - (const Cell_group &) const;
        Cell_group operator - (const Cell &) const;
        bool operator == (const Cell_group &) const;
        bool operator != (const Cell_group &) const;
        Cell_group operator &(const Cell_group &) const;
        const Cell &operator [](unsigned int) const;
        const Cell &random_cell () const;
    private:
        std::vector<int> _id_index;
        friend class World;
    };
}