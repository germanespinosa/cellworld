#include <cell_world/message.h>

namespace cell_world {

    void Message::set_body(const json_cpp::Json_base &new_content) {
        body << new_content;
    }

    Message::Message(const std::string &title, const json_cpp::Json_base &body) :
    title(title),
    body(body.to_json()){

    }

    Message::Message(const std::string &title, const std::string &body) :
    title(title),
    body(body){

    }
}