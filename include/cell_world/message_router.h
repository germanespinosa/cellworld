#include <easy_tcp.h>
#include <json_cpp.h>
#include <cell_world/message.h>

#define Add_route(HEADER,TYPE,DESTINATION) { if (message.header == HEADER) { routed = true; try { DESTINATION(message.get_body<TYPE>()); } catch (...) { failed_route(message); } }}
#define Routes(ADD_ROUTES) bool route(const cell_world::Message &message) override { bool routed = false; ADD_ROUTES; return routed; }
namespace cell_world {
    struct Message_router : easy_tcp::Service {

        virtual bool route(const cell_world::Message &) = 0;
        virtual void failed_route(const Message &);
        virtual void failed_message (const std::string &);
        virtual void unrouted_message(const Message &);
        void on_incoming_data(const std::string &) override;
    };
}