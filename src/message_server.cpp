#include<cell_world/message_server.h>
#include <iostream>


using namespace std;
namespace cell_world{

    void Message_service::failed_route(const Message &m) {
        cerr << "Server: failed to route message " << m << endl;
    }

    void Message_service::failed_message(const string &d) {
        cerr << "Server: failed parsing data '" << d << "' to message" << endl;
    }


    void Message_service::unrouted_message(const Message &) {

    }

    void Message_service::on_incoming_data(const string &data) {
        bool msg_converted = false;
        Message message;
        try {
            data >> message;
            msg_converted = true;
        } catch (...) {
            failed_message(data);
        }
        if (msg_converted) {
            if (!route(message)) unrouted_message(message);
        }
    }

    bool Message_service::send_message(const Message &message) {
        return send_data(message.to_json());
    }
}