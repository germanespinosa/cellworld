#include <cell_world/paths.h>

using namespace std;

namespace cell_world {
    Paths::Paths(const Graph &g) :
            cells(g.cells),
            next_move(cells.size() * cells.size(), Coordinates{0,0}) {
    }

    Move Paths::get_move(const Cell &s, const Cell &d) const {
        int si = _index(s,d);
        if (si == Not_found) return Move{0, 0};
        return next_move[si];
    }

    bool Paths::operator==(const Paths &p) const {
        if (next_move.size() != p.next_move.size()) return false;
        for (unsigned int i = 0; i < next_move.size(); i++) {
            if (next_move[i] != p.next_move[i]) return false;
        }
        return true;
    }

    int Paths::_index(const Cell &s, const Cell &d) const {
        int s_ind = cells.find(s);
        if (s_ind == Not_found) return Not_found;
        int d_ind = cells.find(d);
        if (d_ind == Not_found) return Not_found;
        return s_ind * (int) cells.size() + d_ind;
    }

    bool Paths::set_move(const Cell &s, const Cell &d, const Move &move) {
        int m_index = _index(s,d);
        if (m_index==Not_found) return false;
        next_move [m_index] = move;
        return true;
    }

    Paths Path_builder::get_euclidean(const Graph &g) {
        Paths paths(g);
        for (const Cell &s:g.cells){
            Coordinates s_coordinates = s.coordinates;
            for (const Cell &d:g.cells){
                if (s==d) {
                    paths.set_move(s,d,{0,0});
                } else {
                    Location d_location = d.location;
                    Cell_group s_connections = g[s];
                    if (s_connections.empty()) {
                        paths.set_move(s, d, {0, 0});
                    } else {
                        double min_distance = s_connections[0].location.dist(d_location);
                        int best_option = 0;
                        for (unsigned int i = 1; i < s_connections.size(); i++) {
                            double distance = s_connections[i].location.dist(d_location);
                            if (min_distance > distance) {
                                best_option = i;
                                min_distance = distance;
                            }
                        }
                        Move best_move = s_connections[best_option].coordinates - s_coordinates;
                        paths.set_move(s, d, best_move);
                    }
                }
            }
        }
        return paths;
    }

    Paths Path_builder::get_manhattan(const Graph &g) {
        Paths paths(g);
        for (const Cell &s:g.cells) {
            Coordinates s_coordinates = s.coordinates;
            for (const Cell &d:g.cells) {
                if (s == d) {
                    paths.set_move(s, d, {0, 0});
                } else {
                    auto sp = g.get_shortest_path(s,d);
                    if (sp.empty()){
                        paths.set_move(s, d, {0, 0});
                    } else {
                        Move move = sp[sp.size()-2].coordinates - s_coordinates;
                        paths.set_move(s, d, move);
                    }
                }
            }
        }
        return paths;
    }
}