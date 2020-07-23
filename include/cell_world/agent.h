#pragma once
#include <cell_world/core.h>
#include <cell_world/state.h>
#include <cell_world/chance.h>
#include <cell_world/connection.h>
#include <atomic>
#include <optional>

namespace cell_world{

    enum Agent_status_code{
        Running,
        Finished
    };

    struct Agent : json_cpp::Json_object{
        Agent();
        virtual const Cell &start_episode(unsigned int) = 0;
        virtual Move get_move(const Model_state &) = 0;
        virtual Agent_status_code update_state(const Model_state &) = 0;
        virtual void end_episode(const Model_state &) = 0;
        const Agent_state &state() const;
    private:
        std::optional<std::reference_wrapper<Agent_state>> _state;
        friend class Model;
    };

}