#include <cell_world/agent.h>

using namespace std;
namespace cell_world {
    Agent::Agent() {
    }
    const Agent_state &Agent::state() const {
        return *_state;
    }
}