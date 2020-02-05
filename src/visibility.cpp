#include<visibility.h>
#include<iostream>

using namespace std;
using namespace cell_world;

vector<Cell> Visibility::_line (const Cell &source, const Cell &destination) const{
    vector<Cell> l;
    double dist = source.location.dist(destination.location);
    for (unsigned int i = 0; i < _cell_group.size() ; i++)
        if (_cell_group[i].location.dist(source.location,destination.location) <= .5001 )
            if ( (_cell_group[i].location.dist(source.location) < dist) && (_cell_group[i].location.dist(destination.location) < dist) )
                l.push_back(_cell_group[i]);

    for (unsigned int i = 1; i < l.size() ; i++){
        unsigned int ii = i;
        while ( ii > 0 && source.location.dist(l[ii].location) < source.location.dist(l[ii].location)) std::swap(l[i],l[ii--]);
    }
    return l;
}

uint32_t Visibility::_visibility_index(const uint32_t s, const uint32_t d) const{
    return s * _cell_group.size() + d;
}

void Visibility::_set_visibility(const uint32_t s, const uint32_t  d, const uint8_t is_visible){
    _visibility[_visibility_index(s,d)] = is_visible;
    _visibility[_visibility_index(d,s)] = is_visible;
}

bool Visibility::_get_visible_cells (Cell_group & r, uint32_t cell_id) const{
    r.clear();
    if (_cell_group[cell_id].occluded) return false;
    for (unsigned int i=0;i<_cell_group.size();i++)
        if ((cell_id!=i) && (_visibility[_visibility_index(cell_id,i)] == Visible::Visible)) r.add(_cell_group[i]);
    return true;
}

bool Visibility::_get_visibility(const uint32_t s, const uint32_t d){
    unsigned int i = _visibility_index(s,d);
    if (_visibility[i] != Visible::Not_checked) return _visibility[i] == Visible::Visible;
    if (_cell_group[s].occluded || _cell_group[d].occluded){
        _set_visibility(s,d,Visible::Not_visible);
        return false;
    }
    vector<Cell> l = _line(_cell_group[s],_cell_group[d]);
    for (unsigned int ii = 0; ii < l.size() ; ii++){
        if (!_get_visibility(s,l[ii].id)){
            _set_visibility(s,d,Visible::Not_visible);
            return false;
        };
    }
    _set_visibility(s,d,Visible::Visible);
    return true;
}

void Visibility::reset(const Cell_group &cg) {
    _cell_group = cg;
    _visibility = (uint8_t*) calloc(_cell_group.size()*_cell_group.size(), sizeof(uint8_t));
    for (unsigned int s = 0; s < _cell_group.size() ; s++){
        for (unsigned int d = 0; d < _cell_group.size() ; d++){
            if (s!=d) _get_visibility(s,d);
        }
    }
    _visible_cells.clear();
    for (unsigned int s = 0; s < _cell_group.size() ; s++){
        Cell_group cg;
        _get_visible_cells(cg,s);
        _visible_cells.push_back(cg);
    }
    free(_visibility);
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
