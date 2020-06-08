#include <paths.h>
#include <fstream>
#include <connection.h>

using namespace std;

namespace cell_world {

    Paths::Paths(const Graph &g, Paths::Path_type d) :
            type(d),
            _cells(g.nodes) {
        _init(_cells.size());
        if (d == Path_type::shortest) {
            for (unsigned int i = 0; i < _cells.size(); i++) {
                for (unsigned int j = 0; j < _cells.size(); j++) {
                    if (i == j) continue;
                    auto sp = g.get_shortest_path(_cells[i], _cells[j]);
                    if (sp.size() > 0) {
                        _next_move[i][j] = sp[sp.size() - 2].coordinates - sp[sp.size() - 1].coordinates;
                    }
                }
            }
        } else {
            for (unsigned int i = 0; i < _cells.size(); i++) {
                for (unsigned int j = 0; j < _cells.size(); j++) {
                    if (i == j) continue;
                    auto cnn = g[_cells[i]];
                    unsigned int closest = 0;
                    double closest_distance = _cells[j].location.dist(cnn[0].location);
                    for (unsigned int c = 1; c < cnn.size(); c++) {
                        double distance = _cells[j].location.dist(cnn[c].location);
                        if (distance < closest_distance) {
                            closest_distance = distance;
                            closest = c;
                        }
                    }
                    _next_move[i][j] = cnn[closest].coordinates - _cells[i].coordinates;
                }
            }
        }
    }

    Move Paths::get_move(const Cell &s, const Cell &d) const {
         int si = _cells.find(s);
        if (si == Not_found) return Move{0, 0};
         int di = _cells.find(d);
        if (di == Not_found) return Move{0, 0};
        return _next_move[si][di];
    }

    bool Paths::save(const string &name) const {
        string file_path = name + "_" + _type_string(type) + _extension;
        std::ofstream file;
        file.open(file_path.c_str());
        for (auto &nmv:_next_move) {
            Connection_pattern cp(nmv);
            file << cp.save_to_string() << endl;
        }
        return false;
    }

    bool Paths::save() const {
        return save(_name);
    }

    void Paths::_init(unsigned int size) {
        _next_move = vector<vector<Move>>(size, vector<Move>(size, Move{0, 0}));
    }

    std::string Paths::_type_string(Paths::Path_type type) {
        if (type == Path_type::euclidean)
            return "euclidean";
        return "shortest";
    }

    bool Paths::operator==(const Paths &p) const {
        if (_next_move.size() != p._next_move.size()) return false;
        for (unsigned int i = 0; i < _next_move.size(); i++) {
            if (_next_move[i].size() != p._next_move[i].size()) return false;
            for (unsigned int j = 0; j < _next_move[i].size(); j++) {
                if (_next_move[i][j] != p._next_move[i][j]) return false;
            }
        }
        return true;
    }
}