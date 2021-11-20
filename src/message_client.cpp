#include <cell_world/message_client.h>
#include <cell_world/core.h>
#include <stdexcept>

using namespace std;

namespace cell_world{

    bool Message_client::send_message(const Message &message) {
        return send_data(message.to_json());
    }

    void Message_client::received_data(const std::string &data) {
        try {
            auto message = json_cpp::Json_create<Message>(data);
            if (!route(message))
                unrouted(message);
        } catch (...) {
            failed_message(data);
        }
    }

    void Message_client::failed_message(const std::string &d) {
        cerr << "Client: failed parsing data '" << d << "' to message" << endl;
    }

    bool Message_client::contains(const std::string &header) {
        if (messages.empty()) return false;
        for (auto &message:messages) if (message.header == header) return true;
        return false;
    }

    Message Message_client::get_message(const string &header) {
        if (messages.empty()) throw std::runtime_error("message not found: " + header);
        auto header_match = [header](const Message& m)->size_t{
            return m.header == header;
        };
        auto it = find_if(messages.begin(), messages.end(), header_match);
        if ( it == messages.end()) throw std::runtime_error("message not found: " + header);
        Message res_message = *it;
        messages.erase(it);
        return res_message;
    }

    Message Message_client::get_last_message(const string &header) {
        if (!contains(header)) throw std::runtime_error("message not found: " + header);
        Message message;
        while  (contains(header)) message = get_message(header);
        return message;
    }

    void Message_client::unrouted(const Message &message) {
        messages.push_back(message);
    }

    bool Message_client::route(const Message &) {
        return false;
    }

    void Message_client::failed_route(const Message &) {

    }
}