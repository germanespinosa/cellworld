#include <cell_world/graph.h>
#include <cell_world/chance.h>
#include <cmath>

using namespace std;

namespace cell_world {

    Graph::Graph(const Cell_group &cell_group) :
            nodes(cell_group.free_cells()),
            connections(nodes.size()){
    }

    const Cell_group &Graph::operator[](const Cell &c) const {
        return connections[nodes.find(c)];
    }

    Cell_group &Graph::operator[](const Cell &c) {
        return connections[nodes.find(c)];
    }

    const Cell_group &Graph::operator[](unsigned int index) const {
        return connections[index];
    }

    Cell_group &Graph::operator[](unsigned int index) {
        return connections[index];
    }

    unsigned int Graph::size() const {
        return nodes.size();
    }

    Cell_group Graph::get_shortest_path(const Cell &s, const Cell &d, bool shuffle) const {
        struct Node {
             int parent;
            unsigned int cell_index;
        };
        vector<bool> visited(nodes.size(), false);
        vector<Node> candidates;
        Cell_group path;
         int cell_index = nodes.find(s);
        candidates.push_back({Not_found, (unsigned int) cell_index});
        unsigned int i = 0;
        visited[cell_index] = true;
        while (!connections[candidates[i].cell_index].contains(d)) {
            auto conns = shuffle ? connections[candidates[i].cell_index].random_shuffle()
                                 : connections[candidates[i].cell_index];
            for (unsigned int c = 0; c < conns.size(); c++) {
                unsigned int candidate_index = nodes.find(conns[c]);
                if (!visited[candidate_index]) {
                    candidates.push_back({( int) i, candidate_index});
                    visited[candidate_index] = true;
                }
            }
            if (++i == candidates.size()) return path; // no possible path;
        }
        path.add(d);
        while (candidates[i].parent != Not_found) {
            path.add(nodes[candidates[i].cell_index]);
            i = candidates[i].parent;
        }
        path.add(s);
        return path;
    }

    std::vector<Graph> Graph::get_sub_graphs() {
        Cell_group cg;
        return get_sub_graphs(cg);
    }

    double Graph::get_entropy() {
        vector<int> visible_cell_count;
        for (auto &_connection : connections) {
            visible_cell_count.push_back(_connection.size());
        }
        return entropy(histogram(visible_cell_count));
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

    std::ostream &operator<<(std::ostream &out, const Graph &g) {
        out << "{ \"cells\": " << g.nodes << ", \"connections\": [";
        for (unsigned int i = 0; i < g.connections.size(); i ++) {
            auto &cnn = g.connections[i];
            out << (i?", ":"") << "[";
            for (unsigned int j = 0; j < cnn.size(); j ++) {
                auto cell_index =  g.nodes.find(cnn[j]);
                out << (j?", ":"") << cell_index;
            }
            out << "]";
        }
        out << "]}";
        return out;
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

    vector<Graph> Graph::get_sub_graphs(Graph &gates, Graph &options) {
        vector<Graph> graphs;
        gates.clear();
        options.clear();
        options.add(nodes);
        if (connections.empty()) return graphs;
        unsigned int offset = 0;
         int node_index = Not_found;
        for (; offset < nodes.size() && node_index == Not_found; offset++) {
            if (!gates.nodes.contains(nodes[offset])) node_index = offset; // finds the first node that is not a gate
        }
        while (node_index != Not_found) {
            Cell_group sub_graph_nodes;
            sub_graph_nodes.add(nodes[node_index]); // adds the first node
            Cell_group bridges;
            unsigned int lc = 1; // starts the search on the second node of the sub_graph
            while (node_index != Not_found) {
                sub_graph_nodes += connections[node_index]; // adds all the connections to the selected node;
                node_index = Not_found;
                for (; lc < sub_graph_nodes.size() && node_index == Not_found; lc++) {
                    if (gates.nodes.contains(sub_graph_nodes[lc])) {
                        bridges.add(sub_graph_nodes[lc]);
                    } else {
                        node_index = nodes.find(sub_graph_nodes[lc]);
                    }
                }
            }
            graphs.emplace_back(sub_graph_nodes);
            auto &graph = graphs.back();
            for (unsigned int b = 0; b < bridges.size(); b++) {
                graph.add(bridges[b]);
                gates.connect(bridges[b], bridges - bridges[b]);
            }
            for (unsigned int b = 0; b < graph.nodes.size(); b++) {
                options.connect(graph.nodes[b], bridges - graph.nodes[b]);
            }
            for (unsigned int b = 0; b < graph.size(); b++) {
                graph.connect(graph.nodes[b], (*this)[graph.nodes[b]]);
            }
            for (; offset < nodes.size() && node_index == Not_found; offset++) {
                if (!gates.nodes.contains(nodes[offset])) { // make sure is not a gate
                    node_index = offset;
                    for (unsigned int g = 0; g < graphs.size() && node_index != Not_found; g++) {
                        if (graphs[g].nodes.contains(nodes[node_index])) {
                            node_index = Not_found;
                        }
                    }
                }
            }
        }
        for (unsigned int i = 0; i < gates.size(); i++) {
            auto &gate = gates.nodes[i];
            auto &cnn = (*this)[gate];
            for (unsigned int j = 0; j < cnn.size(); j++) {
                if (gates.nodes.contains(cnn[j])) options[gate].add(cnn[j]);
            }
        }
        return graphs;
    }

    bool Graph::add(const Cell &node) {
        if (nodes.add(node)) {
            connections.emplace_back();
            return true;
        } else return false;

    }

    void Graph::connect(const Cell &node, const Cell_group &connections) {
        if (!nodes.contains(node)) return;
        for (unsigned int i = 0; i < connections.size(); i++)
            if (nodes.find(connections[i]) != Not_found) //only adds connection to nodes in the graph
                (*this)[node] += connections[i];
    }

    std::vector<Graph> Graph::get_sub_graphs(Cell_group &gates) {
        Graph g(gates);
        return get_sub_graphs(g);
    }

    std::vector<Graph> Graph::get_sub_graphs(Graph &gates) {
        Graph options(nodes);
        return get_sub_graphs(gates, options);
    }

    Graph Graph::operator!() const {
        Graph inv(nodes);
        for (unsigned int i = 0; i < nodes.size(); i++)
            for (unsigned int j = 0; j < connections[i].size(); j++)
                inv[connections[i][j]].add(nodes[i]);
        return inv;
    }

    void Graph::connect(Graph &graph) {
        for (unsigned int i = 0; i < graph.nodes.size(); i++) {
            connect(graph.nodes[i], graph[i]);
        }
    }

    Graph &Graph::operator=(const Graph &graph) = default;

    void Graph::clear() {
        connections = json_cpp::Json_vector<Cell_group>(nodes.size());
    }

    bool Graph::operator==(const Graph &g) const {
        if (size() != g.size()) return false;
        if (g.nodes != nodes) return false;
        for (unsigned int i = 0; i < connections.size(); i++) {
            if (connections[i] != g.connections[g.nodes.find(nodes[i])]) return false;
        }
        return true;
    }

    bool Graph::remove(const Cell &c) {
         int index = nodes.find(c);
        if (index == Not_found) return false;
        nodes.remove(c);
        connections.erase(connections.begin() + index);
        return true;
    }

    std::vector<Coordinates> Graph::get_connectors(const Cell &cell) {
        auto c = connections[nodes.find(cell)];
        std::vector<Coordinates> cons;
        for (unsigned int i = 0; i < c.size(); i++) cons.push_back(c[i].coordinates - cell.coordinates);
        return cons;
    }

    bool Graph::add(const Cell_group &c) {
        bool r = true;
        for (unsigned int i = 0; i < c.size(); i++) r = r && add(c[i]);
        return r;
    }

    Graph Graph::invert() const {
        Graph g(nodes);
        for (unsigned int i = 0; i < nodes.size(); i++) {
            auto &conn = connections[i];
            for (unsigned int j = 0; j < nodes.size(); j++) {
                if (!conn.contains(nodes[j])) {
                    g[i].add(nodes[j]);
                }
            }
        }
        return g;
    }

    Graph::Graph() = default;
}