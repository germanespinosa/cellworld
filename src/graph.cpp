#include <cell_world/graph.h>
#include <cell_world/chance.h>
#include <cmath>
#include <queue>

using namespace std;

namespace cell_world {

    Graph::Graph(const Cell_group &cell_group):
    json_cpp::Json_vector<Cell_group>(cell_group.size()),
    cells(cell_group){
    }

    float Graph::get_entropy() {
        vector<unsigned int> connection_counts(this->size(), 0);
        for (unsigned int i=0; i<this->size(); i++){
            connection_counts[i] = (*this)[i].size();
        }
        return weights_entropy(connection_counts);
    }

    bool Graph::add(const Cell &cell) {
        if (cells.add(cell)) {
            emplace_back();
            return true;
        } else return false;

    }

    void Graph::connect(const Cell &node, const Cell_group &connections) {
        if (!cells.contains(node)) return;
        for (unsigned int i = 0; i < connections.size(); i++)
            if (cells.find(connections[i]) != Not_found) //only adds connection to nodes in the graph
                (*this)[node] += connections[i];
    }

    Graph Graph::operator!() const {
        Graph inv(cells);
        for (unsigned int i = 0; i < size(); i++) {
            const Cell_group &node = at(i);
            for (unsigned int j = 0; j < node.size(); j++) {
                inv[node[j]].add(cells[i]);
            }
        }
        return inv;
    }

    Graph Graph::invert() const {
        Graph g(cells);
        for (unsigned int i = 0; i < size(); i++) {
            auto &conn = at(i);
            for (unsigned int j = 0; j < size(); j++) {
                if (!conn.contains(cells[j])) {
                    g.at(i).add(cells[j]);
                }
            }
        }
        return g;
    }

    void Graph::connect(const Graph &graph) {
        for (unsigned int i = 0; i < size(); i++) {
            connect(cells[i], graph.at(i));
        }
    }

    Graph &Graph::operator=(const Graph &graph) = default;

    bool Graph::operator==(const Graph &g) const {
        if (g.cells != cells) return false;
        for (unsigned int i = 0; i < size(); i++) {
            if (at(i) != g[cells[i]]) return false;
        }
        return true;
    }

    std::vector<Coordinates> Graph::get_connectors(const Cell &cell) {
        auto c = at(cells.find(cell));
        std::vector<Coordinates> cons;
        for (unsigned int i = 0; i < c.size(); i++) cons.push_back(c[i].coordinates - cell.coordinates);
        return cons;
    }

    bool Graph::add(const Cell_group &c) {
        bool r = true;
        for (unsigned int i = 0; i < c.size(); i++) r = r && add(c[i]);
        return r;
    }

    const Cell_group &Graph::operator[](const Cell &cell) const {
        return at(cells.find(cell));
    }

    Cell_group &Graph::operator[](const Cell &cell) {
        return at(cells.find(cell));;
    }

    const Cell_group &Graph::operator[](unsigned int i) const {
        return at(i);
    }

    Cell_group &Graph::operator[](unsigned int i) {
        return at(i);
    }

    Graph::Graph() = default;

    Cell_group Graph::get_shortest_path(const Cell &s, const Cell &d, bool shuffle) const {
        struct Node {
            int parent;
            unsigned int cell_index;
        };
        Cell_group path;
        if (s==d) {
            path.add(s);
            return path;
        }
        vector<bool> visited(cells.size(), false);
        vector<Node> candidates;
        int cell_index = cells.find(s);
        candidates.push_back({Not_found, (unsigned int) cell_index});
        unsigned int i = 0;
        visited[cell_index] = true;
        while (! at(candidates[i].cell_index).contains(d)) {
            auto conns = shuffle ? at(candidates[i].cell_index).random_shuffle()
                                 : at(candidates[i].cell_index);
            for (unsigned int c = 0; c < conns.size(); c++) {
                unsigned int candidate_index = cells.find(conns[c]);
                if (!visited[candidate_index]) {
                    candidates.push_back({( int) i, candidate_index});
                    visited[candidate_index] = true;
                }
            }
            if (++i == candidates.size()) return path; // no possible path;
        }
        path.add(d);
        while (candidates[i].parent != Not_found) {
            path.add(cells[candidates[i].cell_index]);
            i = candidates[i].parent;
        }
        path.add(s);
        return path;
    }


    json_cpp::Json_vector<Graph> Graph::get_sub_graphs() {
        Cell_group cg;
        return get_sub_graphs(cg);
    }

    json_cpp::Json_vector<Graph> Graph::get_sub_graphs(Cell_group &gates) {
        Graph g(gates);
        return get_sub_graphs(g);
    }

    json_cpp::Json_vector<Graph> Graph::get_sub_graphs(Graph &gates) {
        Graph options(cells);
        return get_sub_graphs(gates, options);
    }

    json_cpp::Json_vector<Graph> Graph::get_sub_graphs(Graph &gates, Graph &options) {
        json_cpp::Json_vector<Graph> graphs;
        options.add(cells);
        if (empty()) return graphs;
        unsigned int offset = 0;
        int node_index = Not_found;
        for (; offset < cells.size() && node_index == Not_found; offset++) {
            if (!gates.cells.contains(cells[offset])) node_index = offset; // finds the first node that is not a gate
        }
        while (node_index != Not_found) {
            Cell_group sub_graph_nodes;
            sub_graph_nodes.add(cells[node_index]); // adds the first node
            Cell_group bridges;
            unsigned int lc = 1; // starts the search on the second node of the sub_graph
            while (node_index != Not_found) {
                sub_graph_nodes += at(node_index); // adds all the connections to the selected node;
                node_index = Not_found;
                for (; lc < sub_graph_nodes.size() && node_index == Not_found; lc++) {
                    if (gates.cells.contains(sub_graph_nodes[lc])) {
                        bridges.add(sub_graph_nodes[lc]);
                    } else {
                        node_index = cells.find(sub_graph_nodes[lc]);
                    }
                }
            }
            graphs.emplace_back(sub_graph_nodes);
            auto &graph = graphs.back();
            for (unsigned int b = 0; b < bridges.size(); b++) {
                graph.add(bridges[b]);
                gates.connect(bridges[b], bridges - bridges[b]);
            }
            for (unsigned int b = 0; b < graph.size(); b++) {
                options.connect(graph.cells[b], bridges - graph.cells[b]);
            }
            for (unsigned int b = 0; b < graph.size(); b++) {
                graph.connect(graph.cells[b], (*this)[graph.cells[b]]);
            }
            for (; offset < size() && node_index == Not_found; offset++) {
                if (!gates.cells.contains(cells[offset])) { // make sure is not a gate
                    node_index = offset;
                    for (unsigned int g = 0; g < graphs.size() && node_index != Not_found; g++) {
                        if (graphs[g].cells.contains(cells[node_index])) {
                            node_index = Not_found;
                        }
                    }
                }
            }
        }
        for (unsigned int i = 0; i < gates.size(); i++) {
            auto &gate = gates.cells[i];
            auto &cnn = (*this)[gate];
            for (unsigned int j = 0; j < cnn.size(); j++) {
                if (gates.cells.contains(cnn[j])) options[gate].add(cnn[j]);
            }
        }
        return graphs;
    }

    json_cpp::Json_vector<float> Graph::get_centrality(int depth) {
        json_cpp::Json_vector<float> centrality(cells.size(), 1);
        for (int i=0; i<depth; i++){
            vector<float> iteration_centrality(cells.size(), 0);
            for (unsigned int cell_id=0; cell_id<cells.size(); cell_id ++){
                for (const Cell &connection:(*this)[cell_id]) {
                    iteration_centrality[cell_id] += centrality[connection.id];
                }
            }
            float total = 0;

            for (auto c:iteration_centrality) total += c;

            for (unsigned int cell_id=0; cell_id<cells.size(); cell_id ++) {
                centrality[cell_id] = iteration_centrality[cell_id] / total;
            };
        }
        return centrality;
    }

    bool Graph::is_connected(const Cell &src, const Cell &dst) const {
        if (src==dst) {
            return true;
        }
        vector<bool> visited(cells.size(), false);
        queue<unsigned int> pending;
        unsigned int src_index = cells.find(src.id);
        visited[src_index] = true;
        unsigned int current = src_index;
        for (const Cell &c : (*this)[current]) {
            pending.push(cells.find(c.id));
        }
        while (!(*this)[current].contains(dst)){
            if (pending.empty()) return false;
            current = pending.front();
            pending.pop();
            visited[current] = true;
            for (const Cell &c : (*this)[current]) {
                auto index = cells.find(c.id);
                if (!visited[index]) {
                    pending.push(cells.find(c.id));
                }
            }
        }
        return true;
    }
}