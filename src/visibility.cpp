#include <cell_world/visibility.h>

using namespace std;

namespace cell_world {

    Graph Visibility::create_graph(const Cell_group &cell_group) {
        if (cell_group.size() == 0) return Graph();
        Cell_group occlusions = cell_group.occluded_cells();
        auto free_cells = cell_group.free_cells(); // filters occluded
        Graph vi(free_cells);
        for (unsigned int s = 0; s < vi.size(); s++) { //only not occluded
            auto &source = free_cells[s];
            vi[source].add(source); //cell is visible to itself
            for (unsigned int d = s + 1; d < free_cells.size(); d++) { //only not occluded
                auto &destination = free_cells[d];
                double dist = source.location.dist(destination.location); // distance between source & destination
                bool blocked = false;
                for (unsigned int i = 0; i < occlusions.size() && !blocked; i++) {
                    auto &c = occlusions[i];
                    if (c.location.dist(source.location, destination.location) <= (c.cell_type == Square ? .7072 : .5001) &&
                        c.location.dist(source.location) < dist &&
                        c.location.dist(destination.location) < dist) {
                        blocked = true;
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
}