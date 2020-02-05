#pragma once
#include <core.h>
#include <probabilities.h>
#include <connection.h>
namespace cell_world{

    enum Agent_status{
        Started,
        Action_pending,
        Action_ready,
        Finished
    };

    struct Agent_action{
        Agent_action() = default;
        Agent_action(Connection_pattern,Probabilities);
        Connection_pattern destinations;
        Probabilities probabilities;
        bool load(const std::string&);
        bool save(const std::string&) const;
    private:
        const std::string _extension = ".action";
    };

    struct Agent_type{
        std::string name;
        uint32_t version;
    };

    struct Agent_data{
        Agent_type type;
        Agent_status status;
        Color color;
        Cell cell;
    };

    struct State{
        uint32_t iteration;
        std::vector<Agent_data> agents_data;
        int32_t find(const std::string&) const;
    };

    struct Agent{
        explicit Agent(Agent_type);
        virtual const Cell &start_episode(const State &) = 0;
        virtual void update_state(const State &) = 0;
        virtual Agent_action &get_action() = 0;
        virtual void end_episode(const State &) = 0;
        const Cell &cell() const;
        void set_status(Agent_status);
        void set_color(Color);
    private:
        Agent_data data;
        friend class Model;
    };

}