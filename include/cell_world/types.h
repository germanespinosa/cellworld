#pragma once
#include <json_cpp.h>
#ifndef M_E
# define M_E		2.7182818284590452354	/* e */
#endif

#ifndef M_PI
# define M_PI		3.14159265358979323846	/* pi */
#endif

namespace cell_world {
    const int Not_found = -1;

    using Json_string_vector = json_cpp::Json_vector<std::string>;
    using Json_int_vector = json_cpp::Json_vector<int>;
    using Json_bool_vector = json_cpp::Json_vector<bool>;
    using Json_float_vector = json_cpp::Json_vector<float>;
    using Json_unsigned_int_vector = json_cpp::Json_vector<unsigned int>;
}