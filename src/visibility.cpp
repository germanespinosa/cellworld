#include<visibility.h>
#include<iostream>

using namespace std;
using namespace cell_world;

Graph Visibility::create_graph(const Cell_group &cell_group) {
    L("Graph Visibility::create_graph() start");
    if (cell_group.size()==0) return Graph();
    Cell_group occlusions;
    for (unsigned int i = 0; i < cell_group.size(); i++) if (cell_group[i].occluded) occlusions.add(cell_group[i]);
    Graph vi(cell_group); // filters occluded
    for (unsigned int s = 0; s < vi.nodes.size(); s++){ //only not occluded
        auto &source = vi.nodes[s];
        vi[source].add(source);
        for (unsigned int d = s + 1; d < vi.nodes.size(); d++) { //only not occluded
            auto &destination = vi.nodes[d];
            double dist = source.location.dist(destination.location); // distance between source & destination
            bool blocked = false;
            for (unsigned int i = 0; i < occlusions.size() && !blocked; i++) {
                auto &c = occlusions[i];
                if ( c.location.dist(source.location, destination.location) <= .7072 && //(c.cell_type == Square ? .7072 : .5001) &&
                     c.location.dist(source.location) < dist &&
                     c.location.dist(destination.location) < dist ) {
                    blocked = true;
                }
            }
            if (!blocked) {
                vi[source].add(destination);
                vi[destination].add(source);
            }
        }
    }
    L("Graph Visibility::create_graph() end");
    return vi;
}
