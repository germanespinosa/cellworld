#include <cell_world/state.h>
#include <iostream>

using namespace std;

namespace cell_world {
    Agent_state::Agent_state() {
    }
    Agent_state::Agent_state(unsigned int agent_index, Cell cell):
            agent_index(agent_index), iteration(0), cell(cell){
    }
}