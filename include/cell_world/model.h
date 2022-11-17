#pragma once
#include <cell_world/world.h>
#include <cell_world/agent.h>
#include <cell_world/visibility.h>
#include <cell_world/map.h>

namespace cell_world{
    struct Model
    {
        Model(const Cell_group &, unsigned int);
        explicit Model(const Cell_group &);
        Model &add_agent(Agent_base &);
        virtual bool update();
        virtual bool update(const Progress &);
        virtual void start_episode();
        void restart_episode();
        virtual void end_episode();
        const Model_state &get_state() const;
        void set_state(Model_state &);
        void set_public_state(const Model_public_state &);
        void run();
        Map map;
        Model_state &state;
    protected:
        Model_state _state;
        std::vector<std::reference_wrapper<Agent_base>> _agents;
    };
}