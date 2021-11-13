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
        try {
            data >> _message;
            msg_converted = true;
        } catch (...) {
            failed_message(data);
        }
        if (msg_converted) {
            _routed = false;
            routes();
            if (!_routed) unrouted_message(_message);
        }
    }

}