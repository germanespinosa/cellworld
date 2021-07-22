#include <cell_world/map.h>

using namespace std;

namespace cell_world{
    Map::Map(const Cell_group &group)
            : cells(group) {
        if (group.empty()) return;
        int max_x, min_x , min_y , max_y;
        max_x = min_x = group[0].coordinates.x;
        min_y = max_y = group[0].coordinates.y;
        for (unsigned int i = 0; i < group.size(); i++) {
            int x = group[i].coordinates.x;
            int y = group[i].coordinates.y;
            if (x<min_x) min_x = x;
            if (x>max_x) max_x = x;
            if (y<min_y) min_y = y;
            if (y>max_y) max_y = y;
        }
        coordinates.push_back({min_x,min_y});
        coordinates.push_back({max_x,max_y});
        base_x = min_x;
        base_y = min_y;
        columns = max_x-min_x + 1;
        rows = max_y-min_y + 1;
        _coordinate_index = vector<int>(columns * (rows),Not_found);
        for (unsigned int i = 0; i < group.size(); i++) {
            _coordinate_index[_index(group[i].coordinates)] = i;
        }
    }

    int Map::find(const Coordinates &c) const {
        auto index = _index(c);
        if (index<0 || index>=(int)_coordinate_index.size()) return Not_found;
        return _coordinate_index[index];
    }

    int Map::find(const Location &l) const {
        auto index = _index(l);
        if (index<0 || index>=(int)_coordinate_index.size()) return Not_found;
        return _coordinate_index[index];
    }

    const Cell &Map::operator[](const Coordinates &c) const {
        return cells[find(c)];
    }

    const Cell &Map::operator[](const Location &l) const {
        return cells[find(l)];
    }

    int Map::_index(const Location &l) const {
        double min_distance = l.dist(cells[0].location);
        unsigned int min_index = 0;
        for (Cell const &cell:cells) {
            auto distance = l.dist(cell.location);
            if (min_distance > distance){
                min_index = cell.id;
                min_distance = distance;
            }
        }
        return (int)min_index;

    }

    int Map::_index(const Coordinates &c) const {
        int x = c.x - base_x;
        if (x<0 || x>=columns) return Not_found;
        int y = c.y - base_y;
        if (y<0 || y>=rows) return Not_found;
        return (x) + (y) * columns;
    }
}