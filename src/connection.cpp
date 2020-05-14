#include <connection.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <utility>

using namespace std;
using namespace cell_world;

std::vector<Coordinates> Connection_pattern::get_candidates(Coordinates coordinate) const {
    std::vector<Coordinates> c;
    for (auto &p:pattern) c.push_back(coordinate+p);
    return c;
}

const Coordinates &Connection_pattern::operator[](uint32_t index) {
    return pattern[index];
}

uint32_t Connection_pattern::size() const {
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

Connection_pattern &Connection_pattern::operator=(const Connection_pattern &cp) {
    if (&cp != this) {
        pattern.clear();
        for (auto p:cp.pattern) pattern.push_back(p);
    }
    return *this;
}

bool Connection_pattern::load_from_string(const std::string &line) {
    istringstream ss(line);
    pattern.clear();
    while (!ss.eof()) {
        int16_t cx = -1000,cy = -1000;
        ss >> cx;
        ss >> cy;
        if (cx==(int16_t)((int8_t)cx)) pattern.push_back({(int8_t) cx, (int8_t) cy});
        else return false;
    }
    return true;
}

std::string Connection_pattern::save_to_string() const {
    std::stringstream fmt;
    for (auto &p:pattern)
        fmt << (int16_t)p.x << " " << (int16_t)p.y << " ";
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
                n[s].add(n.nodes[destination_index]);
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

Connection_pattern &Connection_pattern::operator=(const string &s) {
    stringstream s_stream(s);
    pattern.clear();
    while (s_stream.good()) {
        string substr;
        getline(s_stream, substr, ';'); //get first string delimited by comma
        Coordinates c{0,0};
        c = substr;
        pattern.push_back(c);
    }
    return *this;
}
