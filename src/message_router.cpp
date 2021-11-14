#include<cell_world/message_router.h>
#include <iostream>

using namespace std;
namespace cell_world{

    void Message_router::failed_route(const Message &m) {
        cerr << "failed to route message " << m << endl;
    }

    void Message_router::failed_message(const string &d) {
        cerr << "failed casting data '" << d << "' to message" << endl;
    }

    void Message_router::unrouted_message(const Message &) {

    }

    void Message_router::on_incoming_data(const string &data) {
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

}