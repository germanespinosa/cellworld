#pragma once
#include <json_cpp.h>

namespace cell_world {
    struct Message : json_cpp::Json_object {
        Message () = default;
        Message (const std::string &header, const std::string &body = "");
        Message (const std::string &header, const json_cpp::Json_base &body);
        template <class T>
        T get_body() const{
            if constexpr (std::is_same_v<T, std::string>) {
                return body;
            }
            T x;
            json_cpp::Json_object_wrapper wx(x);
            body >> wx;
            return x;
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