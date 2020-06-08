#pragma once
#include <core.h>
#include <world.h>
#include <agent.h>
#include <visibility.h>

namespace cell_world{
    struct Execution_log{
        explicit Execution_log(unsigned int);
        void add_agent();
        void start_coordinates(unsigned int, Coordinates);
        void end_value(unsigned int, double);
        void set_value(unsigned int, unsigned int, double);
        void set_coordinates(unsigned int, unsigned int, Coordinates);
        std::vector<std::vector<Coordinates>> trajectories;
        std::vector<std::vector<double>> values;
        unsigned int iterations;
        std::vector<unsigned int> _last_trajectory;
        std::vector<unsigned int> _last_value;
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
        Model(Cell_group &, unsigned int);
        explicit Model(Cell_group &);
        void add_agent(Agent &);
        bool try_update();
        bool update();
        std::vector<Agent_data> get_agents_data();
        void start_episode();
        void start_episode(unsigned int);
        void end_episode();
        State &get_state(unsigned int);
        State &get_state();

        unsigned int iteration;
        unsigned int iterations;
        Cell_group cells;
        void run();
        void run(unsigned int);
        Status status;
        bool finished{};
        Mode mode;
        Map map;
        Graph visibility;
        Execution_log log;
        State state;
    protected:
        std::vector<std::reference_wrapper<Agent>> _agents;
        unsigned int _message_group;
        unsigned int _current_turn;
        bool _try_update_simultaneous();
        bool _try_update_turn();
    friend class Simulation;
    };
}