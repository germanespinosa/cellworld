#include<visibility.h>
#include<iostream>

using namespace std;
using namespace cell_world;

void Visibility::reset(const Cell_group &cg) {
    _cell_group = cg;
    reset();
}

const Cell_group &Visibility::operator[](const Cell& cell) const {
    return _visible_cells[cell.id];
}

double Visibility::get_entropy() {
    vector<int> visible_cell_count;
    for (uint32_t i=0;i<_cell_group.size();i++){
        if (!_cell_group[i].occluded) visible_cell_count.push_back(_visible_cells[i].size());
    }
    return entropy(histogram(visible_cell_count));
}

Visibility::Visibility(const Cell_group &cg) {
    reset(cg);
}

Visibility::Visibility() = default;


void Visibility::reset() {
    double limit = .5001;
    L("Visibility::reset() start");
    if (_cell_group.cell_type == Square) {
        limit = .7072;
        L("Visibility::reset() using square cells");
    } else {
        L("Visibility::reset() using circle cells");
    }

    _visible_cells = vector<Cell_group> (_cell_group.size());
    for (unsigned int s = 0; s < _cell_group.size() ; s++){
        auto &source =_cell_group[s];
        if (!source.occluded) {
            for (unsigned int d = 0; d < _cell_group.size(); d++) {
                auto &destination =_cell_group[d];
                if (s != d && !destination.occluded) {
                    double dist = source.location.dist(destination.location);
                    bool blocked = false;
                    for (unsigned int i = 0; i < _cell_group.size() && !blocked; i++) {
                        auto &c =_cell_group[i];
                        if (c.occluded &&
                            c.location.dist(source.location, destination.location) <= limit &&
                            c.location.dist(source.location) < dist &&
                            c.location.dist(destination.location) < dist) {
                            blocked = true;
                        }
                    }
                    if (!blocked)  _visible_cells[s].add(destination);
                }
            }
        }
    }
    L("Visibility::reset() end");
}