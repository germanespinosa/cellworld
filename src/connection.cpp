#include <connection.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <utility>

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

bool Connections::save(const std::string &name) const{
    string file_path = name + _extension;
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
}

Connections::Connections(const Cell_group &cell_group):
    cells(cell_group){
}

Connections::Connections(const Cell_group &cell_group, const Connection_pattern &pattern) :
        cells(cell_group){
    reset(pattern);
}

void Connections::reset(const Connection_pattern &pattern) {
    clear();
    _id_index.clear();
    Map map(cells);
    for (uint32_t i = 0 ; i<cells.size(); i++) {
        Connection c(cells[i]);
        _connections.push_back(c);
        while (cells[i].id >=_id_index.size()) _id_index.push_back(Not_found);
        _id_index[cells[i].id]=i;
    }
    for (uint32_t source = 0; source < cells.size(); source++){
        if ( !cells[source].occluded ) {
            for (auto c : pattern.get_candidates(cells[source].coordinates)) {
                int32_t destination = map.find(c);
                if (destination != Not_found && !cells[destination].occluded) {
                    add(cells[source],cells[destination]);
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

bool Connection_pattern::load(const std::string& name) {
    string file_name = name + _extension;
    pattern.clear();
    std::ifstream file;
    file.open(file_name.c_str());
    if (!file.good()) return false;
    string line;
    while (getline(file, line)){
        istringstream ss(line);
        int16_t cx,cy;
        ss >> cx;
        ss >> cy;
        pattern.push_back({(int8_t)cx,(int8_t)cy});
    }
    return true;
}

bool Connection_pattern::save(const std::string& name) const {
    string file_name = name + _extension;
    std::ofstream file;
    file.open(file_name.c_str());
    if (!file.good()) return false;
    for (const auto c : pattern){
        file
                << (int16_t)c.x << " "
                << (int16_t)c.y << std::endl;
    }
    return true;
}

Connection_pattern::Connection_pattern() = default;

Connection_pattern::Connection_pattern(std::vector<Coordinates>pattern)
:pattern(std::move(pattern)){

}
