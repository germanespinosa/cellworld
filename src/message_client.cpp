#include <cell_world/message_client.h>

using namespace std;

namespace cell_world{

    bool Message_client::send_message(const Message &message) {
        return send_data(message.to_json());
    }

    void Message_client::received_data(const std::string &data) {
        try {
             messages.push(json_cpp::Json_create<Message>(data));
        } catch (...) {
            failed_message(data);
        }
    }

    void Message_client::failed_message(const std::string &d) {
        cerr << "Client: failed parsing data '" << d << "' to message" << endl;
    }
}