#include <cell_world/state.h>
#include <iostream>

using namespace std;

namespace cell_world {
    Agent_state::Agent_state() {
    }
    Agent_state::Agent_state(unsigned int iteration, Cell cell):
        iteration(iteration), cell(cell){
    }
}