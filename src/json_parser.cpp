#include <cell_world.h>
#include <iostream>

using namespace std;
using namespace cell_world;

#define CELL_PARSE_ERROR throw logic_error("Cell format should be [id, type, coordinates, location, occluded, value, icon, direction]")
#define LOCATION_PARSE_ERROR throw logic_error("Location format should be [x,y]")
#define COORDINATES_PARSE_ERROR throw logic_error("Coordinates format should be [x,y]")
#define WORLD_PARSE_ERROR throw logic_error("World format should be [name, [cell_1,cell_2,...,cell_n], connection_pattern]")
#define CONNECTION_PATTERN_PARSE_ERROR throw logic_error("Connection pattern format should be [coord_1,coord_2,...,coord_n]")
#define STRING_PARSE_ERROR throw logic_error("decimal error converting to string")
#define INT_PARSE_ERROR throw logic_error("decimal error converting to int")
#define DOUBLE_PARSE_ERROR throw logic_error("decimal error converting to double")

namespace cell_world {

    static Output_format output_format = Output_format::List;

    void set_output_format(Output_format o){
        output_format = o;
    }

    std::ostream &operator<<(std::ostream &out, const Coordinates &c) {
        if (output_format == Output_format::List) {
            out << "[" << c.x << "," << c.y << "]";
        } else {
            out << "{\"x\":" << c.x << ",\"y\":" << c.y << "}";
        }
        return out;
    }

    std::ostream &operator<<(std::ostream &out, const Connection_pattern &cp) {
        out << "[";
        bool first = true;
        for (auto const &c: cp.pattern) {
            if (!first) out << ",";
            out << c;
            first = false;
        }
        out << "]";
        return out;
    }

    std::ostream &operator<<(std::ostream &out, const Location &l) {
        if (output_format == Output_format::List) {
            out << "[" << l.x << "," << l.y << "]";
        } else {
            out << "{\"x\":" << l.x << ",\"y\":" << l.y << "}";
        }
        return out;
    }

    std::ostream &operator<<(std::ostream &out, const Cell &c) {
        if (output_format == Output_format::List) {
            out << "[" << c.id << ", "
                << (int) c.cell_type << ", "
                << c.coordinates << ", "
                << c.location << ", "
                << c.occluded << ", "
                << c.value << ", "
                << (int) c.icon << ", "
                << c.direction << "]";
        } else {
            out << "{\"id\":" << c.id << ", "
                << "\"type\":" << (int) c.cell_type << ", "
                << "\"coordinates\":" << c.coordinates << ", "
                << "\"location\":" << c.location << ", "
                << "\"occluded\":" << c.occluded << ", "
                << "\"value\":" << c.value << ", "
                << "\"icon\":" << (int) c.icon << ", "
                << "\"direction\":" << c.direction << "}";
        }
        return out;
    }


    std::ostream &operator<<(std::ostream &out, const World &world) {
        if (output_format == Output_format::List) {
            out << "[\"" << world.name << "\","
                << world.connection_pattern << ",[";
            for (unsigned int i=0; i<world._cells.size(); i++){
                if (i) out << ",";
                out << world._cells[i];
            }
            out << "]]";
        } else {
            out << "{\"name\": \"" << world.name << "\","
            << "\"connection_pattern\": " << world.connection_pattern << ","
            << "\"cells\": [";
            for (unsigned int i=0; i<world._cells.size(); i++){
                if (i) out << ",";
                out << world._cells[i];
            }
            out << "]}";
        }
        return out;
    }

    std::istream &operator>>(istream &i, Coordinates &coord) {
        char c;
        int x,y;
        char t;
        t = skip_blanks(i);
        if (t =='[') {
            x = read_int(c, i);
            if (skip_blanks(c, i) != ',') COORDINATES_PARSE_ERROR;
            y = read_int(c, i);
            if (skip_blanks(c, i) != ']') COORDINATES_PARSE_ERROR;
            coord.x = x;
            coord.y = y;
        } else if ( t == '{') {
            do {
                c=' ';
                string name = read_string(i);
                if (( name != "x" && name != "y" ) || (skip_blanks(i)!=':')) COORDINATES_PARSE_ERROR;
                int value = read_int(c, i);
                if (name=="x") coord.x = value;
                else coord.y = value;
                t = skip_blanks(c,i);
            } while (t == ',');
            if (t!='}') COORDINATES_PARSE_ERROR;
        } else COORDINATES_PARSE_ERROR;
        return i;
    }

    std::istream &operator>>(istream &i, Location &loc) {
        char c;
        double x,y;
        char t;
        t = skip_blanks(i);
        if (t =='[') {
            x = read_double(c, i);
            if (skip_blanks(c, i) != ',') LOCATION_PARSE_ERROR;
            y = read_double(c, i);
            if (skip_blanks(c, i) != ']') LOCATION_PARSE_ERROR;
            loc.x = x;
            loc.y = y;
        } else if ( t == '{') {
            do {
                c = ' ';
                string name = read_string(i);
                if (( name != "x" && name != "y" ) || (skip_blanks(i)!=':')) LOCATION_PARSE_ERROR;
                double value = read_double(c, i);
                if (name=="x") loc.x = value;
                else loc.y = value;
                t = skip_blanks(c,i);
            } while (t == ',');
            if (t!='}') LOCATION_PARSE_ERROR;
        } else LOCATION_PARSE_ERROR;
        return i;
    }
    std::istream &operator>>(istream &i, Cell &cell) {
        char c,t;
        t = skip_blanks( i);
        if (t =='[') { ;
            cell.id = read_int(c, i);

            if (skip_blanks(c, i) != ',') CELL_PARSE_ERROR;
            cell.cell_type = (Cell_type) read_int(c, i);

            if (skip_blanks(c, i) != ',') CELL_PARSE_ERROR;
            i >> cell.coordinates;


            if (skip_blanks(i) != ',') CELL_PARSE_ERROR;
            i >> cell.location;


            if (skip_blanks(i) != ',') CELL_PARSE_ERROR;
            cell.occluded = read_int(c, i);

            t = skip_blanks(c, i);
            if (t == ']') return i;
            if (t != ',') CELL_PARSE_ERROR;
            cell.value = read_double(c, i);

            t = skip_blanks(c, i);
            if (t == ']') return i;
            if (t != ',') CELL_PARSE_ERROR;
            cell.icon = (cell_world::Icon) read_int(c, i);

            t = skip_blanks(c, i);
            if (t == ']') return i;
            if (t != ',') CELL_PARSE_ERROR;
            i >> cell.direction;

            if (skip_blanks(i) != ']') CELL_PARSE_ERROR;
        } else if (t =='{'){
            while (t!='}'){
                c=' ';
                string name = read_string(i);
                if (skip_blanks(i)!=':') CELL_PARSE_ERROR;
                if (name == "id"){
                    cell.id = read_int(c,i);
                } else if (name == "type") {
                    cell.cell_type = (Cell_type) read_int(c,i);
                } else if (name == "coordinates") {
                    i >> cell.coordinates;
                } else if (name == "location") {
                    i >> cell.location;
                } else if (name == "occluded") {
                    cell.occluded = read_int(c,i);
                } else if (name == "value") {
                    cell.value = read_double(c,i);
                } else if (name == "icon") {
                    cell.icon = (Icon)read_int(c,i);
                } else if (name == "direction") {
                    i >> cell.direction;
                } else CELL_PARSE_ERROR;
                t = skip_blanks(c,i);
            }
        } else CELL_PARSE_ERROR;
        return i;
    }

    std::istream &operator>>(istream &i, Connection_pattern &cp) {
        char t;
        if (skip_blanks(i)!='[') CONNECTION_PATTERN_PARSE_ERROR;
        cp.pattern.clear();
        do {
            Coordinates cnn;
            i >> cnn;
            cp.pattern.push_back(cnn);
            t = skip_blanks(i);
        } while (t == ',');
        if (t != ']') CONNECTION_PATTERN_PARSE_ERROR;
        return i;
    }

    std::istream &operator>>(istream &i, World &w) {
        char c,t;
        t = skip_blanks( i);
        if (t =='[') {
            w.name = read_string(i);
            if (skip_blanks(i)!=',') WORLD_PARSE_ERROR;
            i >> w.connection_pattern;
            if (skip_blanks(i)!=',') WORLD_PARSE_ERROR;
            if (skip_blanks(i)!='[') WORLD_PARSE_ERROR;
            do {
                Cell cell;
                i >> cell;
                w.add(cell);
                t = skip_blanks(i);
            } while (t == ',');
            if (t != ']') WORLD_PARSE_ERROR;
            if (skip_blanks(i) != ']') WORLD_PARSE_ERROR;
        } else if(t=='{'){
            while (t!='}'){
                c=' ';
                string name = read_string(i);
                if (skip_blanks(i)!=':') CELL_PARSE_ERROR;
                if (name == "name"){
                    w.name = read_string(i);
                } else if (name == "cells") {
                    if (skip_blanks(i)!='[') WORLD_PARSE_ERROR;
                    do {
                        Cell cell;
                        i >> cell;
                        w.add(cell);
                        t = skip_blanks(i);
                    } while (t == ',');
                    if (t != ']') WORLD_PARSE_ERROR;
                } else if (name == "connection_pattern") {
                    i >> w.connection_pattern;
                } else CELL_PARSE_ERROR;
                t = skip_blanks(c,i);
            }
            if (skip_blanks(t,i) != '}') WORLD_PARSE_ERROR;
        } else WORLD_PARSE_ERROR;
        return i;
    }

    string read_string(istream &i) {
        char c;
        if (skip_blanks(i)!='"') STRING_PARSE_ERROR;
        string s;
        do {
            i >> c;
            if  (c != '"') s += c;
        } while (c != '"');
        return s;
    }

    double read_double(char &c, istream &i) {
        do {
            i >> c;
        } while (c == ' ');  // read all blank spaces at the beginning
        string s;
        bool negative = false;
        if (c=='-'){
            negative = true;
            i>>c;
        }
        bool decimal = false;
        while ((c >='0' && c <='9') || c=='.'){ //copy all numbers to s
            if (c=='.'){
                if (decimal) DOUBLE_PARSE_ERROR;
                decimal = true;
            }
            s += c;
            i >> c;
        }
        if (s.empty()) DOUBLE_PARSE_ERROR;
        if (negative)
            return -stod(s);
        else
            return stod(s);
    }

    int read_int(char &c, istream &i) {
        do {
            i >> c;
        } while (c == ' ');  // read all blank spaces at the beginning
        string s;
        bool negative = false;
        if (c=='-'){
            negative = true;
            i>>c;
        }
        while (c >='0' && c <='9') { //copy all numbers to s
            s += c;
            i >> c;
        }
        if (s.empty()) INT_PARSE_ERROR;
        if (negative)
            return -stoi(s);
        else
            return stoi(s);
    }

    bool read_to(char d, istream &i) {
        char c;
        do {
            i >> c;
        } while (c == ' ');  // read all blank spaces
        return c==d;
    }

    char skip_blanks(char c, istream &i) {
        while (c == ' '){
            i >> c;
        }
        return c;
    }

    char skip_blanks(istream &i) {
        char c;
        do {
            i >> c;
        } while (c == ' ' || c == '\r' || c == '\n');
        return c;
    }
}