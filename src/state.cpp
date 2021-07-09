#include <cell_world/state.h>
#include <iostream>

using namespace std;

namespace cell_world {
    Agent_public_state::Agent_public_state(unsigned int agent_index):
        agent_index(agent_index), iteration(0){
    }

    Agent_public_state::Agent_public_state():
            agent_index(0), iteration(0) {
    }

    Agent_internal_state_container::Agent_internal_state_container(size_t size):
        _mem_blob(size ? size + 4 : 0,0){ } // if stateless then 0 else adds 4 bytes for boundary alignment

    void *Agent_internal_state_container::get_address() {
        return _mem_blob.data();
    }

    bool cell_world::Progress::operator==(const Progress &p) const {
        return this->iteration == p.iteration && this->turn == p.turn;
    }

    bool cell_world::Progress::operator<(const Progress &p) const {
        return this->iteration < p.iteration || (this->iteration == p.iteration && this->turn < p.turn);
    }

    bool cell_world::Progress::operator<=(const Progress &p) const {
        return *this == p || *this < p;
    }

    bool cell_world::Progress::operator>(const Progress &p) const {
        return this->iteration > p.iteration || (this->iteration == p.iteration && this->turn > p.turn);
    }

    bool cell_world::Progress::operator>=(const Progress &p) const {
        return *this == p || *this > p;
    }

    Progress::Progress(unsigned int iteration, unsigned int turn):
    iteration(iteration), turn(turn){
    }

    Progress Model_public_state::get_progress() const {
        return Progress({agents_state[current_turn].iteration,current_turn});
    }
}