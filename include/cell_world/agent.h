#pragma once
#include <cell_world/core.h>
#include <cell_world/chance.h>
#include <cell_world/connection.h>
#include <atomic>

namespace cell_world{
    enum Agent_status{
        Update_pending,
        Action_pending,
        Action_ready,
        Finished
    };

     struct Stochastic_move {
         Stochastic_move(const Connection_pattern&, std::vector<unsigned int>);
         Move get_move();
         Connection_pattern destinations;
         std::vector<unsigned int> chances;
    };

    struct Agent_type{
        std::string name = "";
        unsigned int version = 0;
    };

    struct Agent_data{
        Agent_type type;
        Color color;
        Icon icon = No_icon;
        Cell cell;
    };

    struct State{
        unsigned int iteration;
        unsigned int iterations;
        std::vector<bool> visible;
        std::vector<Agent_data> agents_data;
         int find(const std::string&) const;
    };

    struct Agent_message{
        Agent_type from;
        Agent_type to;
        std::string body;
    };

    struct Agent{
        explicit Agent(Agent_type);
        virtual const Cell &start_episode(unsigned int) = 0;
        virtual Move get_move(const State &) = 0;
        virtual void update_state(const State &) = 0;
        virtual double get_value();
        virtual void end_episode(const State &) = 0;
        virtual void receive_message(const Agent_message&);
        void send_message(const Agent_type&, const std::string &) const;
        void send_message(const std::string &) const;
        const Cell &cell() const;
        void set_status(Agent_status);
        void set_color(Color);
        void set_value(double);
    protected:
        double value;
        Agent_data data;
        Agent_status status;
    private:
        unsigned int _agent_index;
        unsigned int _message_group;
        void _set_cell(const Cell &);
        friend class Model;
        friend struct Agent_broadcaster;
    };

    struct Agent_broadcaster{
        static void add(Agent *agent, unsigned int group);
        static void send(const Agent_message &, unsigned int group);
        static unsigned int new_message_group();
    };

}