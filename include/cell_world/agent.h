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

    struct Agent_base : json_cpp::Json_object{
        Agent_base();
        virtual const Cell &start_episode() = 0;
        virtual Move get_move(const Model_public_state &) = 0;
        virtual Agent_status_code update_state(const Model_public_state &) = 0;
        virtual void end_episode(const Model_public_state &) = 0;
        const Agent_public_state &public_state() const;
    protected:
        virtual size_t get_internal_state_size () = 0;
        virtual void set_internal_state (void *) = 0;
    private:
        void set_public_state (Agent_public_state *s);
        Agent_public_state *_state;
        friend class Model;
    };

    struct Stateless_agent: Agent_base{
        size_t get_internal_state_size() override { return 0 ;};
        void set_internal_state(void *) override {};
    };

    template <class STATE_STRUCT>
    struct Stateful_agent : Agent_base {
        size_t get_internal_state_size () override {
            return sizeof(STATE_STRUCT);
        }
        STATE_STRUCT &internal_state() {
            return *_internal_state;
        }
        void set_internal_state(void *ptr) override {
            _internal_state = (STATE_STRUCT *)ptr;
        }
    private:
        STATE_STRUCT *_internal_state;
    };
}