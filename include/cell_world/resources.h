#pragma once
#include <any>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <json_cpp.h>

namespace cell_world {
    struct Resources {
        static std::string &cache_folder();
        static Resources from(const std::string);
        template <class T>
        T get_resource (){
            try {
                _new_cache = false;
                T o = json_cpp::Json_create<T>(get());
                return o;
            } catch(...){
                remove_cache();
                throw std::logic_error("failed to load content from " + url() + " into variable of type " + typeid(T).name());
            }
        }
        Resources &key(const std::string &);
        Resources &key(int);
        Resources &key(unsigned int);
        Resources &key(const char *);
        std::istream &get();
        std::string url();
        std::string to_string();

    private:
        std::string _file_name();

        std::string _resource;
        std::vector<std::string> _keys;
        std::ifstream _resource_stream;
        bool _new_cache;
        void remove_cache();
        Resources() = default;
    };
}