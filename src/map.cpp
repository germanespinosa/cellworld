#include <cell_world/core.h>
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
        _base.x = min_x;
        _base.y = min_y;
        _columns = max_x-min_x + 1;
        _rows = max_y-min_y + 1;
        _coordinate_index = vector<int>(_columns * (_rows),Not_found);
        for (unsigned int i = 0; i < group.size(); i++) {
            _coordinate_index[_index(group[i].coordinates)] = i;
        }
    }

    int Map::find(const Coordinates &c) const {
        auto index = _index(c);
        if (index<0 || index>=(int)_coordinate_index.size()) return Not_found;
        return _coordinate_index[index];
    }

    const Cell &Map::operator[](const Coordinates &c) const {
        return cells[find(c)];
    }

    int Map::_index(const Coordinates &c) const {
        int x = c.x - _base.x;
        if (x<0 || x>=_columns) return Not_found;
        int y = c.y - _base.y;
        if (y<0 || y>=_rows) return Not_found;
        return (x) + (y) * _columns;
    }
}