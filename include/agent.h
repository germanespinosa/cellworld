#pragma once
#include <core.h>
#include <chance.h>
#include <connection.h>

namespace cell_world{

    using Move  = Coordinates;

    enum Agent_status{
        Started,
        Action_pending,
        Action_ready,
        Finished
    };

     struct Stochastic_move {
         Stochastic_move(const Connection_pattern&, std::vector<uint32_t>);
         Move get_move();
         Connection_pattern destinations;
         std::vector<uint32_t> chances;
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
        virtual const Cell &start_episode() = 0;
        virtual void update_state(const State &) = 0;
        virtual Move get_move() = 0;
        virtual void end_episode(const State &) = 0;
        const Cell &cell() const;
        void set_status(Agent_status);
        void set_color(Color);
    private:
        Agent_data data;
        friend class Model;
    };

}