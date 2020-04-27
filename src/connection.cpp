#include <connection.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <utility>

using namespace std;
using namespace cell_world;
/*
bool Connections::_add(const Cell &source, const Cell &destination) {
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

void Connections::_clear() {
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
    _clear();
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
                    _add(cells[source],cells[destination]);
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
*/
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
    cout << "saving pattern" <<endl;
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

Connection_pattern &Connection_pattern::operator=(const Connection_pattern &cp) {
    if (&cp != this) {
        pattern.clear();
        for (auto p:cp.pattern) pattern.push_back(p);
    }
    return *this;
}

bool Connection_pattern::load_from_string(const std::string &line) {
    L("Connection_pattern::load_from_string(const std::string &) start");
    istringstream ss(line);
    pattern.clear();
    while (!ss.eof()) {
        int16_t cx = -1000,cy = -1000;
        ss >> cx;
        ss >> cy;
        if (cx==(int16_t)((int8_t)cx)) pattern.push_back({(int8_t) cx, (int8_t) cy});
        else return false;
    }
    L("Connection_pattern::load_from_string(const std::string &) end");
    return true;
}

std::string Connection_pattern::save_to_string() const {
    L("Connection_pattern::save_to_string() start");
    std::stringstream fmt;
    for (auto &p:pattern)
        fmt << (int16_t)p.x << " " << (int16_t)p.y << " ";
    L("Connection_pattern::save_to_string() end");
    return fmt.str();
}

Graph Connection_pattern::get_graph(const Cell_group &cg) const {
    Graph n(cg); // filter occluded
    Map map(n.nodes);
    for (uint32_t s = 0; s < n.size(); s++){
        auto &source = n.nodes[s];
        for (auto c : get_candidates(source.coordinates)) {
            int32_t destination_index = map.find(c);
            if (destination_index != Not_found) {
                n[source].add(n.nodes[destination_index]);
            }
        }
    }
    return n;
}

Connection_pattern Connection_pattern::get_pattern(Cell c, Cell_group cg) {
    Connection_pattern cp;
    for (uint32_t i=0;i< cg.size();i++) cp.pattern.push_back(cg[i].coordinates-c.coordinates);
    return cp;
}

Connection_pattern Connection_pattern::operator=(const string &s) {
    stringstream s_stream(s);
    pattern.clear();
    while (s_stream.good()) {
        string substr;
        getline(s_stream, substr, ';'); //get first string delimited by comma
        Coordinates c;
        c=substr;
        pattern.push_back(c);
    }
    return *this;
}
