#pragma once
#include <deque>
#include <easy_tcp.h>
#include <cell_world/message.h>

namespace cell_world {
    struct Message_client: easy_tcp::Client {
        void received_data(const std::string &) override;
        virtual void failed_message(const std::string &);
        bool contains(const std::string &header);
        bool send_message(const Message &);
        Message get_message (const std::string &header);
        std::deque<Message> messages;
    };
}