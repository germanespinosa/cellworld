#pragma once
#include <json_cpp.h>

namespace cell_world {
    const int Not_found = -1;

    using Json_int_vector = json_cpp::Json_vector<int>;
    using Json_bool_vector = json_cpp::Json_vector<bool>;
    using Json_float_vector = json_cpp::Json_vector<float>;
    using Json_unsigned_int_vector = json_cpp::Json_vector<unsigned int>;
}