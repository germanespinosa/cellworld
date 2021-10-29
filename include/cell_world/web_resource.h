#pragma once
#include <any>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <json_cpp.h>

namespace cell_world {
    struct Web_resource {
        static std::string &cache_folder();
        static Web_resource from(const std::string);
        template <class T>
        T get_resource (){
            return json_cpp::Json_create<T>(get());
        }
        Web_resource &key(const std::string &);
        Web_resource &key(int);
        Web_resource &key(unsigned int);
        Web_resource &key(const char *);
        std::istream &get();
        std::string url();

    private:
        std::string _file_name();

        std::string _resource;
        std::vector<std::string> _keys;
        std::ifstream _resource_stream;

        Web_resource() = default;
    };
}