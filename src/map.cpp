#include <cell_world/map.h>

using namespace std;

namespace cell_world{
    Map::Map(const Cell_group &group)
            : _group(group) {
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
        base_x = min_x;
        base_y = min_y;
        _coordinate_index = vector<vector<int>>(max_x-min_x + 1,vector<int>(max_y-min_y + 1,Not_found));
        for (unsigned int i = 0; i < group.size(); i++) {
            auto x = group[i].coordinates.x-base_x;
            auto y = group[i].coordinates.y-base_y;
            _coordinate_index[x][y] = i;
        }
    }

    int Map::find(const Coordinates &c) const {
        auto x = c.x-base_x;
        auto y = c.y-base_y;
        if (x<0 || x>=(int)_coordinate_index.size()) return Not_found;
        if (y<0 || y>=(int)_coordinate_index[x].size()) return Not_found;
        return _coordinate_index[x][y];
    }

    const Cell &Map::operator[](const Coordinates &c) const {
        return _group[find(c)];
    }
}