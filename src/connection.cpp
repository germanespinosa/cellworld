#include <connection.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>

using namespace std;
using namespace cellworld;

bool Connections::process_eigen_centrality() {
    return process_eigen_centrality(100, 0.000001);
}

bool Connections::process_eigen_centrality(uint32_t max_iterations, double tolerance) {
    double nnodes = (double)_connections.size();
    vector<double> last(_connections.size());
    double err = nnodes * 100;
    for (uint32_t index = 0; index < _connections.size(); index++) _connections[index].eigen_centrality = 1.0/nnodes;
    for (uint32_t iteration = 0; iteration < max_iterations && err > nnodes * tolerance; iteration++){
        for (uint32_t index = 0; index < _connections.size(); index++)
            last[index] = _connections[index].eigen_centrality;
        for (uint32_t n = 0; n < _connections.size(); n++)
            for (uint32_t nbr = 0; nbr < _connections[n].size(); nbr++)
                _connections[_connections[n][nbr]].eigen_centrality += last[n];
        double d=0;
        for (uint32_t index = 0; index < _connections.size(); index++)
            d += pow(_connections[index].eigen_centrality,2);
        d = pow(d,.5);
        double s = d==0?1:1.0/d;
        for (uint32_t index = 0; index < _connections.size(); index++) _connections[index].eigen_centrality *= s;
        err=0;
        for (uint32_t index = 0; index < _connections.size(); index++)
            err += abs(_connections[index].eigen_centrality - last[index]);
    }
    return err < nnodes * tolerance;
}

bool Connections::save(const std::string &filepath) const{
    std::ofstream ofile;
    ofile.open(filepath.c_str());
    for (unsigned int i=0;i<_connections.size();i++){
        if (_connections[i].size()!=0) {
            ofile << i;
            for (unsigned int j = 0; j < _connections[i].size(); j++) {
                ofile << " " << _connections[i][j];
            }
            ofile << std::endl;
        }
    }
    return true;
}


Connections::Connections() {

}

bool Connections::add(uint32_t source, uint32_t destination) {
    while (source >=_connections.size()) _connections.emplace_back();
    return _connections[source].add(destination);
}

bool Connections::remove(uint32_t source, uint32_t destination) {
    return _connections[source].remove(destination);
}

const Connection &Connections::operator[](uint32_t source) const{
//    while (source >=_connections.size()) _connections.emplace_back();
    return _connections[source];
}

uint32_t Connections::size() const {
    return _connections.size();
}

void Connections::clear() {
    _connections.clear();
}

bool Connection::is_connected(uint32_t destination) const{
    for (uint32_t i = 0; i<_connections.size(); i++)
        if (_connections[i]==destination) return true;
    return false;
}

bool Connection::add(uint32_t destination) {
    if (is_connected(destination)) return false;
    _connections.push_back(destination);
    return true;
}

bool Connection::remove(uint32_t destination) {
    std::vector<uint32_t>::iterator it = find(_connections.begin(),_connections.end(),destination);
    if (it == _connections.end()) return false;
    _connections.erase(it);
    return true;
}

uint32_t Connection::size() const {
    return _connections.size();
}

uint32_t Connection::operator[](uint32_t index) const{
    return _connections[index];
}

const std::vector<uint32_t> &Connection::get_all() const{
    return _connections;
}
