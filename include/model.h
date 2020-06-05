#pragma once
#include <core.h>
#include <world.h>
#include <agent.h>
#include <visibility.h>

namespace cell_world{
    struct Execution_log{
        explicit Execution_log(uint32_t);
        void add_agent();
        void start_coordinates(uint32_t, Coordinates);
        void set_value(uint32_t, uint32_t, double);
        void set_coordinates(uint32_t, uint32_t, Coordinates);
        std::vector<std::vector<Coordinates>> trajectories;
        std::vector<std::vector<double>> values;
        uint32_t iterations;
        std::vector<uint32_t> _last_trajectory;
        std::vector<uint32_t> _last_value;
        friend std::ostream &operator<<(std::ostream&, Execution_log const&);
    };

    struct Model
    {
        enum class Mode{
            Simultaneous,
            Turns
        };
        enum class Status{
            Idle,
            Running,
            Stopped,
            Finalized,
        };
        Model(Cell_group &, uint32_t);
        explicit Model(Cell_group &);
        void add_agent(Agent &);
        bool try_update();
        bool update();
        std::vector<Agent_data> get_agents_data();
        void start_episode();
        void start_episode(uint32_t);
        void end_episode();
        State &get_state(uint32_t);
        State &get_state();

        uint32_t iteration;
        uint32_t iterations;
        Cell_group cells;
        void run();
        void run(uint32_t);
        Status status;
        bool finished{};
        Mode mode;
        Map map;
        Graph visibility;
        Execution_log log;
        State state;
    protected:
        std::vector<std::reference_wrapper<Agent>> _agents;
        uint32_t _message_group;
        uint32_t _current_turn;
        bool _try_update_simultaneous();
        bool _try_update_turn();
    friend class Simulation;
    };
}