#pragma once
#include <core.h>

namespace cell_world{
    struct Cell_group{
        Cell_group();
        Cell_group(const cell_world::Cell_group&) = default;
        bool save(const std::string &) const;
        bool save() const;
        bool add(const Cell &);
        bool remove(const Cell &);
        bool toggle(const Cell &);
        bool contains(unsigned int) const;
        bool contains(const Cell &) const;
        void clear();
        unsigned int size() const;
         int find(unsigned int) const;
         int find(const Cell &) const;
        double distance(unsigned int, unsigned int) const;
        static double distance(const Cell&, const Cell&) ;
        std::vector<double> get_distances(const Cell &) const;
        Cell_group random_shuffle() const;
        Cell_group occluded_cells() const;
        Cell_group free_cells() const;
        Cell_group &operator = (const Cell_group &);
        Cell_group &operator += (const Cell_group &);
        Cell_group &operator += (const Cell &);
        Cell_group &operator -= (const Cell_group &);
        Cell_group &operator -= (const Cell &);
        Cell_group operator + (const Cell_group &);
        Cell_group operator + (const Cell &);
        Cell_group operator - (const Cell_group &);
        Cell_group operator - (const Cell &);
        bool operator == (const Cell_group &) const;
        bool operator != (const Cell_group &) const;
        const Cell &operator [](unsigned int) const;
        const Cell &random_cell () const;
        friend std::ostream& operator<<(std::ostream& , const Cell_group& );
    private:
        std::vector<std::reference_wrapper<const Cell>> _cells;
        std::vector<int> _id_index;
        std::string _file_name;
        const Cell & _get_cell(unsigned int) const;
        const std::string _extension = ".group";
        friend class World;
    };

    struct Map{
        explicit Map(const Cell_group &);
        const Cell& operator [](const Coordinates &) const;
         int find (const Coordinates &) const;

    private:
        std::vector<std::vector< int>> _coordinate_index;
        const Cell_group &_group;
    };
}