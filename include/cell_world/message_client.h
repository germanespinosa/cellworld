#pragma once
#include <queue>
#include <easy_tcp.h>
#include <cell_world/message.h>

namespace cell_world {
    struct Message_client: easy_tcp::Client {
        void received_data(const std::string &) override;
        virtual void failed_message(const std::string &);
        bool send_message(const Message &);
        std::queue<Message> messages;
    };
}