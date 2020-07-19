#include <cell_world/connection.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <utility>
#include <cell_world/chance.h>

using namespace std;

namespace cell_world {

    std::vector<Coordinates> Connection_pattern::get_candidates(Coordinates coordinate) const {
        std::vector<Coordinates> c;
        for (auto &p:(*this)) c.push_back(coordinate + p);
        return c;
    }

    Connection_pattern::Connection_pattern() = default;

    Connection_pattern::Connection_pattern(std::vector<Coordinates> pattern)
    {
        for (auto &p: pattern) push_back(p);
    }

    Connection_pattern &Connection_pattern::operator=(const Connection_pattern &cp) {
        if (&cp != this) {
            clear();
            for (auto p:cp) push_back(p);
        }
        return *this;
    }

    Graph Connection_pattern::get_graph(const Cell_group &cg) const {
        Graph n(cg); // filter occluded
        Map map(n.nodes);
        for (unsigned int s = 0; s < n.size(); s++) {
            auto &source = n.nodes[s];
            for (auto c : get_candidates(source.coordinates)) {
                 int destination_index = map.find(c);
                if (destination_index != Not_found) {
                    n[s].add(n.nodes[destination_index]);
                }
            }
        }
        return n;
    }

    Connection_pattern Connection_pattern::get_pattern(Cell c, Cell_group cg) {
        Connection_pattern cp;
        for (unsigned int i = 0; i < cg.size(); i++) cp.push_back(cg[i].coordinates - c.coordinates);
        return cp;
    }

    Move Connection_pattern::random_move() {
        return (*this)[Chance::dice(size())];
    }
}