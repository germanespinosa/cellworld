#include <world.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


using namespace std;
using namespace cellworld;
using namespace ge211;

bool Cell::operator == (const Cell& c) const {
    if (connections.size() != c.connections.size() ||
         id != c.id ||
         coordinates != c.coordinates ||
         location != c.location ||
         occluded != c.occluded){
        return false;
    }
    for (unsigned int i=0; i <connections.size() ; i++)
        if (connections[i] != c.connections[i]) return false;
    return true;
}
    

Cell::Cell (uint32_t id, Coordinates coordinates, Basic_position<double> location, std::vector<uint32_t> connections, double value, bool occluded)
{
    this->id = id;
    this->location = location;
    this->coordinates = coordinates;
    this->connections = connections;
    this->value = value;
    this->occluded=occluded;
}

Cell::Cell(){
    this->id = 0;
    this->location = {0,0};
    this->coordinates = {0,0};
    this->connections = {};
    this->occluded= false;
   
}

void World::connect(vector<Coordinates> connections){
    for (unsigned int i = 0; i < cells.size(); i++){
        for (unsigned int c =0; c < connections.size();c++){
            int cc = find(cells[i].coordinates+connections[c]);
            if (cc>=0){
                if (!cells[cc].occluded) cells[i].connections.push_back(cc);
            }
        }
    }
}

bool World::add(const Cell cell){
    if (cells.size() != cell.id) return false;
    cells.push_back(cell);
    return true;
}
bool World::load(const std::string filepath){
    cells.clear();
    std::ifstream ifile;
    ifile.open(filepath.c_str());
    string line;
    while (getline(ifile, line)){
        istringstream ss(line);
        int16_t cx,cy;
        Cell cell;
        ss >> cell.id;
        ss >> cx;
        ss >> cy;
        ss >> cell.location.x;
        ss >> cell.location.y;
        ss >> cell.value;
        ss >> cell.occluded;
        cell.coordinates.x = cx;
        cell.coordinates.y = cy;
        while (!ss.eof()){
            int32_t ci = -1;
            ss >> ci;
            if (ci>=0)
                cell.connections.push_back(ci);
        }
        if (!add(cell)) return false;
    }
    return true;
}
bool World::save(const std::string filepath) const{
    std::ofstream  ofile;
    ofile.open(filepath.c_str());
    for (unsigned int i=0;i<cells.size();i++){
        ofile << i << " " 
        << (int16_t)cells[i].coordinates.x << " "
        << (int16_t)cells[i].coordinates.y << " "
        << cells[i].location.x << " "
        << cells[i].location.y << " "
        << cells[i].value << " "
        << cells[i].occluded ;
       for (unsigned int c=0;c<cells[i].connections.size();c++){
           ofile << " " << cells[i].connections[c];
       }
       ofile << std::endl;
    }
    return true;
}
uint32_t World::size() const{
    return cells.size();
}

int32_t World::find (const Coordinates& coordinates) const{
    static int32_t *map=NULL;
    if (map == NULL){
        map = (int32_t *) malloc(256*256* sizeof(int32_t));
        for (unsigned int i=0; i<256*256; i++) map[i] = -1;
        for (unsigned int i=0; i<cells.size(); i++) {
            int m=(cells[i].coordinates.x+128)*256 + (cells[i].coordinates.y+128);
            map[m] = i;
        }
    }
/*    for (unsigned int i=0; i<cells.size(); i++)
        if (cells[i].coordinates==coordinates) return i;*/
    int mi=(coordinates.x+128)*256 + (coordinates.y+128);
    return map[mi];
}

Cell &World::operator[](const uint32_t& id){
    return cells[id];
}

Cell &World::operator[](const Coordinates& coordinates){
    return cells[find(coordinates)];
}

double World::distance(const Cell &c0, const Cell &c1) const {
    return sqrt(pow(c1.location.y-c0.location.y,2) + pow(c1.location.x-c0.location.x,2));
}
double World::distance(const Cell  &c0, const Cell &c1, const Cell &c2) const {
    return abs((c2.location.y-c1.location.y) * c0.location.x-(c2.location.x-c1.location.x) *c0.location.y+c2.location.x*c1.location.y-c2.location.y * c1.location.x) / sqrt(pow(c2.location.y-c1.location.y,2)+pow(c2.location.x-c1.location.x,2));
}

