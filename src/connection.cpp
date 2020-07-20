#include <cell_world/connection.h>
#include <cell_world/chance.h>
#include <cell_world/map.h>

using namespace std;

namespace cell_world {

    Move_list Connection_pattern::get_candidates(Coordinates coordinate) const {
        Move_list c;
        for (auto &p:(*this)) c.push_back(coordinate + p);
        return c;
    }

    Connection_pattern::Connection_pattern() = default;

    Connection_pattern &Connection_pattern::operator=(const Connection_pattern &cp) {
        if (&cp != this) {
            clear();
            for (auto p:cp) push_back(p);
        }
        return *this;
    }

    Graph Connection_pattern::get_graph(const Cell_group &cg) const {
        Graph n(cg);
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