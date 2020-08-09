#include <cell_world/visibility.h>

using namespace std;

namespace cell_world {

    Graph Visibility::create_graph(const Cell_group &cell_group) {
        if (cell_group.size() == 0) return Graph();
        Cell_group occlusions = cell_group.occluded_cells();
        Cell_group free_cells = cell_group.free_cells(); // filters occluded
        Graph vi(cell_group);
        for (const Cell &source:free_cells) { //only not occluded
            vi[source].add(source); //cell is visible to itself
            for (const Cell &destination:free_cells) { //only not occluded
                if( destination.id < source.id) continue;
                bool blocked = false;
                if (source!=destination) {
                    double dist = source.location.dist(destination.location); // distance between source & destination
                    for (const Cell &occlusion:occlusions) {
                        if (occlusion.location.dist(source.location, destination.location) <=
                               (occlusion.cell_type == Square ? .7072 : .5001) &&
                                occlusion.location.dist(source.location) < dist &&
                                occlusion.location.dist(destination.location) < dist) {
                            blocked = true;
                            break;
                        }
                    }
                }
                if (!blocked) {
                    vi[source].add(destination);
                    vi[destination].add(source);
                }
            }
        }
        return vi;
    }

    Graph Visibility::invert(const Graph &vi) {
        Graph iv(vi.cells);
        Cell_group free_cells = iv.cells.free_cells();
        for (auto &s:free_cells) {
            for (auto &d:free_cells) {
                if (!vi[s].contains(d)) {
                    iv[s].add(d);
                }
            }
        }
        return iv;
    }
}