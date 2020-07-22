#include <cell_world/connection.h>
#include <cell_world/chance.h>
#include <cell_world/map.h>
#include <cmath>

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

    std::vector<double> Centrality::get_eigen_centrality(Graph &graph) {
        return get_eigen_centrality(graph, 100, 0.000001);
    }

    std::vector<double> Centrality::get_betweenness_centrality(Graph &graph, unsigned int precision) {
        vector<double> result(graph.size(), 0);
        for (unsigned int i = 0; i < graph.nodes.size() - 1; i += (Chance::dice(precision)) + 1) {
            for (unsigned int j = i + 1; j < graph.nodes.size(); j += (Chance::dice(precision)) + 1) {
                auto path = graph.get_shortest_path(graph.nodes[i], graph.nodes[j]);
                for (unsigned int c = 0; c < path.size(); c++) {
                    result[graph.nodes.find(path[c])]++;
                }
            }
        }
        return result;
    }

    std::vector<double> Centrality::get_eigen_centrality(Graph &graph, unsigned int max_iterations, double tolerance) {
        auto nodes = (double) graph.size();
        vector<double> result(graph.size());
        vector<double> last(graph.size());
        double err = nodes * 100;
        for (auto &r : result) r = 1.0 / nodes;
        for (unsigned int iteration = 0; iteration < max_iterations && err > nodes * tolerance; iteration++) {
            last = result;
            for (unsigned int n = 0; n < graph.size(); n++)
                for (unsigned int nbr = 0; nbr < graph[n].size(); nbr++)
                    result[graph.nodes.find(graph[n][nbr])] += last[n];
            double d = 0;
            for (auto &_connection : result) d += pow(_connection, 2);
            d = pow(d, .5);
            double s = d == 0 ? 1 : 1.0 / d;
            for (auto &_connection : result) _connection *= s;
            err = 0;
            for (unsigned int index = 0; index < result.size(); index++)
                err += abs(result[index] - last[index]);
        }
        return result;
    }

    std::vector<double> Centrality::get_betweenness_centrality(Graph &graph) {
        return get_betweenness_centrality(graph, 10);
    }

}