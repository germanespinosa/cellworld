#include <easy_tcp.h>
#include <json_cpp.h>
#include <cell_world/message.h>

#define Add_route(HEADER,TYPE,DESTINATION) { if (_message.header == "#HEADER") { _routed = true; try { DESTINATION(json_cpp::Json_create<TYPE>(_message.body)); } catch (...) { failed_route(_message); } }}

namespace cell_world {
    struct Message_router : easy_tcp::Service {

        virtual void routes() = 0;
        virtual void failed_route(const Message &);
        virtual void failed_message (const std::string &);
        virtual void unrouted_message(const Message &);
        void on_incoming_data(const std::string &) override;
    protected:
        Message _message;
        bool _routed;
    };
}