#include <path.h>

using namespace cell_world;
using namespace std;

Path::Path(const Graph &g, Path::Distance_type d){
//    _cells = g.nodes;
//    _next_move = vector<vector<Move>>(_cells.size(), vector<Move>(_cells.size(),Move{0,0}));
//    if (d == Distance_type::a_star) {
//        for (uint32_t i = 0; i < _cells.size(); i++) {
//            for (uint32_t j = 0; j < _cells.size(); j++) {
//                auto sp = g.get_shortest_path(_cells[i], _cells[j]);
//                if (sp.size() > 0) {
//                    _next_move[i][j] = sp[sp.size() - 1].coordinates - sp[sp.size() - 2].coordinates;
//                }
//            }
//        }
//    } else {
//        for (uint32_t i = 0; i < _cells.size(); i++) {
//            for (uint32_t j = 0; j < _cells.size(); j++) {
//                auto cnn = g.get(_cells[i]);
//                uint32_t closest = 0;
//                double closest_distance = _cells[j].location.dist(cnn[0].location);
//                for (uint32_t c = 1; c < cnn.size(); c++) {
//                    double distance = _cells[j].location.dist(cnn[c].location);
//                    if (distance < closest_distance) {
//                        closest_distance = distance;
//                        closest = c;
//                    }
//                }
//                _next_move[i][j] = cnn[closest].coordinates - _cells[i].coordinates;
//            }
//        }
//    }
}

Move Path::get_move(const Cell &s, const Cell &d) const{
    int32_t si = _cells.find(s);
    if (si == Not_found) return Move{0,0};
    int32_t di = _cells.find(d);
    if (di == Not_found) return Move{0,0};
    return _next_move[si][di];
};