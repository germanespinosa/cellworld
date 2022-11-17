#pragma once
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
        virtual const Cell &start_episode() { return Cell::ghost_cell(); };
        virtual Move get_move(const Model_public_state &) { return {0,0};};
        virtual Agent_status_code update_state(const Model_public_state &) { return Agent_status_code::Running; };
        virtual void end_episode(const Model_public_state &) {};
        const Agent_public_state &public_state() const;
    protected:
        virtual size_t get_internal_state_size () = 0;
        virtual void set_internal_state (Agent_internal_state_container &, bool) = 0;
    private:
        void set_public_state (Agent_public_state *s);
        Agent_public_state *_state;
        friend class Model;
    };

    struct Stateless_agent: Agent_base{
        size_t get_internal_state_size() override { return 0 ;};
        void set_internal_state(Agent_internal_state_container &, bool) override {};
    };

    using Agent_internal_state = json_cpp::Json_object;

    template <class STATE_STRUCT>
    struct Stateful_agent : Agent_base {
        static_assert(std::is_base_of<Agent_internal_state, STATE_STRUCT>::value, "STATE_STRUCT must inherit from "
                                                                         "Agent_internal_state");
        size_t get_internal_state_size () override {
            return sizeof(STATE_STRUCT);
        }
        STATE_STRUCT &internal_state() {
            return *_internal_state;
        }
        void set_internal_state(Agent_internal_state_container &c, bool init) override {
            auto place = (void *)c.get_address();
            if (init) {
                _internal_state = new(place) STATE_STRUCT();
            } else {
                _internal_state =  (STATE_STRUCT *) place;
            }
        }
        void set_internal_state(STATE_STRUCT &s) {
            *_internal_state = s;
        }
    private:
        STATE_STRUCT *_internal_state;
    };
}