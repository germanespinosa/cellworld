#include<visibility.h>
#include<iostream>

using namespace std;
using namespace cell_world;

vector<Cell> Visibility::_line (const Cell &source, const Cell &destination) const{
    vector<Cell> l;
    double dist = _world.distance(source,destination);
    for (unsigned int i = 0; i < _world.size() ; i++)
        if (_world.distance(_world[i],source,destination) <= .5001 )
            if ( (_world.distance(_world[i],source) < dist) && (_world.distance(_world[i],destination) < dist) )
                l.push_back(_world[i]);

    for (unsigned int i = 1; i < l.size() ; i++){
        unsigned int ii = i;
        while ( ii > 0 && _world.distance(source,l[ii]) < _world.distance(source,l[ii])) std::swap(l[i],l[ii--]);
    }
    return l;
}

uint32_t Visibility::_visibility_index(const uint32_t s, const uint32_t d) const{
    return s * _world.size() + d;
}

void Visibility::_set_visibility(const uint32_t s, const uint32_t  d, const uint8_t is_visible){
    _visibility[_visibility_index(s,d)] = is_visible;
    _visibility[_visibility_index(d,s)] = is_visible;
}

bool Visibility::_get_visible_cells (Cell_group & r, uint32_t cell_id) const{
    r.clear();
    if (_world[cell_id].occluded) return false;
    for (unsigned int i=0;i<_world.size();i++)
        if ((cell_id!=i) && (_visibility[_visibility_index(cell_id,i)] == Visible::Visible)) r.add(_world[i]);
    return true;
}

bool Visibility::_get_visibility(const uint32_t s, const uint32_t d){
    unsigned int i = _visibility_index(s,d);
    if (_visibility[i] != Visible::Not_checked) return _visibility[i] == Visible::Visible;
    if (_world[s].occluded || _world[d].occluded){
        _set_visibility(s,d,Visible::Not_visible);
        return false;
    }
    vector<Cell> l = _line(_world[s],_world[d]);
    for (unsigned int ii = 0; ii < l.size() ; ii++){
        if (!_get_visibility(s,l[ii].id)){
            _set_visibility(s,d,Visible::Not_visible);
            return false;
        };
    }
    _set_visibility(s,d,Visible::Visible);
    return true;
}

Visibility::Visibility( World &world) : _world(world){
    reset();
}

void Visibility::reset() {
    _visibility = (uint8_t*) calloc(_world.size()*_world.size(), sizeof(uint8_t));
    for (unsigned int s = 0; s < _world.size() ; s++){
        for (unsigned int d = 0; d < _world.size() ; d++){
            if (s!=d) _get_visibility(s,d);
        }
    }
    _visible_cells.clear();
    for (unsigned int s = 0; s < _world.size() ; s++){
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
    for (uint32_t i=0;i<_world.size();i++){
        if (!_world[i].occluded) visible_cell_count.push_back(_visible_cells[i].size());
    }

    return entropy(histogram(visible_cell_count));
}
