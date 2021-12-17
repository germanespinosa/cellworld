#include <cell_world/agent.h>

using namespace std;
namespace cell_world {
    Agent_base::Agent_base() {
    }
    const Agent_public_state &Agent_base::public_state() const {
        return *_state;
    }

    void Agent_base::set_public_state(Agent_public_state *s) {
        _state = s;
    }

}