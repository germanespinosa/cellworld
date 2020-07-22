#include <cell_world/connection.h>
#include <cell_world/chance.h>
#include <cell_world/map.h>
#include <cmath>

using namespace std;

namespace cell_world {

    Coordinates_list Connection_pattern::get_candidates(Coordinates coordinate) const {
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
        auto free_cells = cg.free_cells();
        Graph graph(free_cells);
        Map map(free_cells);
        int destination_index;
        for (const Cell &cell:free_cells) {
            for (Coordinates &coord : get_candidates(cell.coordinates)) {
                if ((destination_index = map.find(coord)) != Not_found) {
                    graph[cell].add(free_cells[destination_index]);
                }
            }
        }
        return graph;
    }

    Connection_pattern Connection_pattern::get_pattern(Cell c, Cell_group cg) {
        Connection_pattern cp;
        for (unsigned int i = 0; i < cg.size(); i++) cp.push_back(cg[i].coordinates - c.coordinates);
        return cp;
    }

    Move Connection_pattern::random_move() {
        return (*this)[Chance::dice(size())];
    }
/*
    std::vector<double> Centrality::get_eigen_centrality(Graph &graph) {
        return get_eigen_centrality(graph, 100, 0.000001);
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
*/
}