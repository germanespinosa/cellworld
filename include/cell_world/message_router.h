#pragma once
#include <easy_tcp.h>
#include <json_cpp.h>
#include <cell_world/message.h>
#include <regex>

#define Add_route(HEADER, DESTINATION, ...) {                             \
    if (std::regex_match (message.header,std::regex(HEADER))) {           \
        routed = true;                                                    \
        try {                                                             \
                DESTINATION(__VA_OPT__(message.get_body<__VA_ARGS__>())); \
        } catch (...) {                                                   \
            failed_route(message);                                        \
        }                                                                 \
    }                                                                     \
}

#define Routes(ADD_ROUTES) bool route(const cell_world::Message &message) override { bool routed = false; ADD_ROUTES; return routed; }
