#pragma once
#include <json_cpp.h>

namespace cell_world {
    struct Message : json_cpp::Json_object {
        Message () = default;
        Message (const std::string &header, const std::string &body = "");
        Message (const std::string &header, const json_cpp::Json_base &body);
        template <class T>
        T get_body(){
            static_assert(std::is_base_of<json_cpp::Json_base, T>::value, "Type must inherit from Json_base");
            return json_cpp::Json_create<T>(body);
        }
        void set_body(const json_cpp::Json_base &content);
        Json_object_members({
            Add_member(header);
            Add_member(body);
        });
        std::string header;
        std::string body;
    };
}