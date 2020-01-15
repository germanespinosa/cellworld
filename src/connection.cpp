#include <connection.h>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;
using namespace cell_world;

bool Connections::process_eigen_centrality() {
    return process_eigen_centrality(100, 0.000001);
}

bool Connections::process_eigen_centrality(uint32_t max_iterations, double tolerance) {
    auto nodes = (double)_connections.size();
    vector<double> last(_connections.size());
    double err = nodes * 100;
    for (auto & _connection : _connections) _connection.eigen_centrality = 1.0/nodes;
    for (uint32_t iteration = 0; iteration < max_iterations && err > nodes * tolerance; iteration++){
        for (uint32_t index = 0; index < _connections.size(); index++)
            last[index] = _connections[index].eigen_centrality;
        for (uint32_t n = 0; n < _connections.size(); n++)
            for (uint32_t nbr = 0; nbr < _connections[n].connections.size(); nbr++)
                _connections[_id_index[_connections[n].connections[nbr].id]].eigen_centrality += last[n];
        double d=0;
        for (auto & _connection : _connections) d += pow(_connection.eigen_centrality,2);
        d = pow(d,.5);
        double s = d==0?1:1.0/d;
        for (auto & _connection : _connections) _connection.eigen_centrality *= s;
        err=0;
        for (uint32_t index = 0; index < _connections.size(); index++)
            err += abs(_connections[index].eigen_centrality - last[index]);
    }
    return err < nodes * tolerance;
}

bool Connections::save(const std::string &file_path) const{
    std::ofstream file;
    file.open(file_path.c_str());
    for (const auto & _connection : _connections){
        file << _connection.cell.id;
        for (uint32_t j = 0; j < _connection.connections.size(); j++) file << " " << _connection.connections[j].id;
        file << std::endl;
    }
    return true;
}


bool Connections::add(const Cell &source, const Cell &destination) {
    while (source.id >=_id_index.size()) _id_index.push_back(Not_found);
    if (_id_index[source.id]==Not_found) {
        _id_index[source.id] = _connections.size();
        Connection c(source);
        _connections.push_back(c);
    }
    if (_connections[_id_index[source.id]].connections.contains(destination))
        return false;
    _connections[_id_index[source.id]].connections.add(destination);
    return true;
}

uint32_t Connections::size() const {
    return _connections.size();
}

void Connections::clear() {
    _connections.clear();
    _id_index.clear();
}

Connections::Connections(const Cell_group &cell_group):
    _cells(cell_group){

}

Connections::Connections(const Cell_group &cell_group, const Connection_pattern &pattern) :
        _cells(cell_group){
    reset(pattern);
}

void Connections::reset(const Connection_pattern &pattern) {
    clear();
    for (uint32_t i = 0 ; i<_cells.size(); i++) {
        Connection c(_cells[i]);
        _connections.push_back(c);
        while (_cells[i].id >=_id_index.size()) _id_index.push_back(Not_found);
        _id_index[_cells[i].id]=i;
    }
    for (uint32_t source = 0; source < _cells.size(); source++){
        if ( !_cells[source].occluded ) {
            for (auto c : pattern.get_candidates(_cells[source].coordinates)) {
                int32_t destination = _cells.find(c);
                if (destination != Not_found && !_cells[destination].occluded) {
                    add(_cells[source],_cells[destination]);
                }
            }
        }
    }
}

const Connection &Connections::operator[](const Cell &cell) const {
    return _connections[_id_index[cell.id]];
}

const Connection &Connections::operator[](uint32_t source) const{
    return _connections[source];
}


Connection::Connection(const Cell &cell )
    :cell(cell){}

std::vector<Coordinates> Connection_pattern::get_candidates(Coordinates coordinate) const {
    std::vector<Coordinates> c;
    for (auto &p:pattern) c.push_back(coordinate+p);
    return c;
}

const Coordinates &Connection_pattern::operator[](uint32_t index) {
    return pattern[index];
}

uint32_t Connection_pattern::size() {
    return pattern.size();
}
