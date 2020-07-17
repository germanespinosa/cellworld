#include <cell_world.h>
#include <iostream>
#include <utility>
#include <json.h>
#include <core.h>


using namespace std;
using namespace cell_world;

#define STRING_PARSE_ERROR throw logic_error("decimal error converting to string")
#define INT_PARSE_ERROR throw logic_error("decimal error converting to int")
#define DOUBLE_PARSE_ERROR throw logic_error("decimal error converting to double")

namespace cell_world {
    static Output_format output_format = Output_format::List;

    void set_output_format(Output_format o){
        output_format = o;
    }

    std::istream &operator>>(istream &i, Json_base &o) {
        o.json_parse(i);
        return i;
    }

    std::ostream &operator<<(ostream &o, Json_base &j) {
        j.json_write(o);
        return o;
    }

    void Json_object::json_parse(istream &i) {
        Json_parser p;
        json_set_parser(p);
        p.json_parse(i);
    }

    void Json_object::json_write(ostream &o) {
        Json_parser p;
        json_set_parser(p);
        p.json_write(o);
    }


    void Json_parser::json_add_member(std::string name, bool mandatory, bool &variable) {
        _json_descriptors.push_back({std::move(name), mandatory, Json_value_type::Json_Boolean, bool_members.size()});
        bool_members.emplace_back(variable);
    }

    void Json_parser::json_add_member(std::string name, bool mandatory, double &variable) {
        _json_descriptors.push_back({std::move(name), mandatory, Json_value_type::Json_Double, double_members.size()});
        double_members.emplace_back(variable);
    }

    void Json_parser::json_add_member(std::string name, bool mandatory, int &variable) {
        _json_descriptors.push_back({std::move(name), mandatory, Json_value_type::Json_Int, int_members.size()});
        int_members.emplace_back(variable);
    }

    void Json_parser::json_add_member(std::string name, bool mandatory, unsigned int &variable) {
        _json_descriptors.push_back({std::move(name), mandatory, Json_value_type::Json_Unsigned, uint_members.size()});
        uint_members.emplace_back(variable);
    }

    void Json_parser::json_add_member(std::string name, bool mandatory, string &variable) {
        _json_descriptors.push_back({std::move(name), mandatory, Json_value_type::Json_String, string_members.size()});
        string_members.emplace_back(variable);
    }

    void Json_parser::json_add_member(std::string name, bool mandatory, Json_base &variable) {
        _json_descriptors.push_back({std::move(name), mandatory, Json_value_type::Json_object, json_members.size()});
        json_members.emplace_back(variable);
    }

    void Json_parser::json_parse(istream &i) {
        if (Json_util::skip_blanks(i) == '{') {
            Json_util::discard(i);
            string name;
            while (Json_util::skip_blanks(i) != '}') {
                if (!Json_util::read_name(name, i)) throw logic_error("format error: field name");
                _json_set_member_value(_json_descriptor(name), i);
                if (Json_util::skip_blanks(i) != ',') break;
                Json_util::discard(i);
            }
            if (Json_util::skip_blanks(i) != '}') {
                throw logic_error("format error: expecting '}'");
            }
            Json_util::discard(i);
        } else if (Json_util::skip_blanks(i) == '[') {
            Json_util::discard(i);
            bool finished = false;
            for (auto &d:_json_descriptors) {
                if (Json_util::skip_blanks(i) == ']') {
                    finished = true;
                }
                if (!finished) {
                    _json_set_member_value(d, i);
                    if (Json_util::skip_blanks(i) != ',' && Json_util::skip_blanks(i) != ']') throw logic_error("format error: expecting ',' or ']'");
                    if (Json_util::skip_blanks(i) == ',') Json_util::discard(i);
                } else if (d.mandatory) {
                    throw logic_error("missing mandatory fields");
                }
            }
            if (Json_util::skip_blanks(i) != ']') {
                throw logic_error("format error: expecting ']'");
            }
            Json_util::discard(i);
        } else  {
            throw logic_error("format error: expecting '{' or '[");
        }
    }

    Json_parser::Json_descriptor &Json_parser::_json_descriptor(const string &name) {
        for (auto &d:_json_descriptors) {
            if (d.name == name) return d;
        }
        throw logic_error("format error: descriptor for field '" + name + "' not found");
    }

    void Json_parser::_json_set_member_value(Json_descriptor &d, istream &i) {
        switch (d.type) {
            case Json_value_type::Json_Boolean :
                bool_members[d._index].get() = Json_util::read_int(i);
                break;
            case Json_value_type::Json_Unsigned :
                uint_members[d._index].get() = Json_util::read_int(i);
                break;
            case Json_value_type::Json_Int :
                int_members[d._index].get() = Json_util::read_int(i);
                break;
            case Json_value_type::Json_Double :
                double_members[d._index].get() = Json_util::read_double(i);
                break;
            case Json_value_type::Json_String :
                string_members[d._index].get() =  Json_util::read_string(i);
                break;
            case Json_value_type::Json_object :
                json_members[d._index].get().json_parse(i);
                break;
        }
    }

    void Json_parser::json_write(ostream &o) {
        o << (output_format==Output_format::Object?"{":"[");
        bool first = true;
        for (auto &d:_json_descriptors) {
            if (!first) o << ",";
            first = false;
            if (output_format == Output_format::Object) o << "\"" << d.name << "\": ";
            switch (d.type) {
                case Json_value_type::Json_Boolean:
                    o << bool_members[d._index].get();
                    break;
                case Json_value_type::Json_Unsigned:
                    o << uint_members[d._index].get();
                    break;
                case Json_value_type::Json_Int:
                    o << int_members[d._index].get();
                    break;
                case Json_value_type::Json_Double:
                    o << double_members[d._index].get();
                    break;
                case Json_value_type::Json_String:
                    o << "\"" << string_members[d._index].get() << "\"";
                    break;
                case Json_value_type::Json_object:
                    json_members[d._index].get().json_write(o);
                    break;
            }
        }
        o << (output_format==Output_format::Object?"}":"]");
    }

    void Json_util::discard(istream &i) {
        char c;
        i >> c;
    }

    char Json_util::skip_blanks(istream &i, bool s) {
        i >> ws;
        char c;
        if (s) i >> c; else c = i.peek();
        return c;
    }

    char Json_util::skip_blanks(istream &i) {
        return skip_blanks(i, false);
    }

    std::string Json_util::read_string(istream &i) {
        char c;
        if (skip_blanks(i)!='"') STRING_PARSE_ERROR;
        discard(i);
        string s;
        do {
            i >> c;
            if  (c != '"') s += c;
        } while (c != '"');
        return s;
    }

    double Json_util::read_double(istream &i) {
        char c = skip_blanks(i);
        if (!isdigit(c) && c!='.' && c!='-') DOUBLE_PARSE_ERROR;
        double r;
        i >> r;
        return r;
    }

    int Json_util::read_int(istream &i) {
        char c = skip_blanks(i);
        if (!isdigit(c) && c!='-') INT_PARSE_ERROR;
        int r;
        i >> r;
        return r;
    }

    bool Json_util::read_name(string &s, istream &i) {
        s = read_string(i);
        return skip_blanks(i, true) == ':';
    }
}


