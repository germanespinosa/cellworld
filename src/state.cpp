#include <cell_world/state.h>
#include <iostream>

using namespace std;

namespace cell_world {
    Agent_public_state::Agent_public_state() {
    }
    Agent_public_state::Agent_public_state(unsigned int agent_index):
        agent_index(agent_index){
    }
    Agent_internal_state::Agent_internal_state(size_t size):
        _mem_blob(size){
    }

    void *Agent_internal_state::get_address() {
        return _mem_blob.data();
    }
}