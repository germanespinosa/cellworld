#pragma once
#include <cell_world/core.h>
#include <cell_world/state.h>
#include <cell_world/chance.h>
#include <cell_world/connection.h>
#include <atomic>

namespace cell_world{

    enum Agent_status_code{
        Running,
        Finished
    };

    struct Agent{
        Agent();
        virtual const Cell &start_episode(unsigned int) = 0;
        virtual Move get_move(const Model_state &) = 0;
        virtual Agent_status_code update_state(const Model_state &) = 0;
        virtual void end_episode(const Model_state &) = 0;
        virtual Agent_state &get_state_reference()=0;
    private:
        unsigned int _agent_index;
        friend class Model;
    };

}