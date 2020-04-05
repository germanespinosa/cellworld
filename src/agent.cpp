#include <agent.h>
#include <utility>

using namespace std;
namespace cell_world {

    const Cell &Agent::cell() const {
        L("Cell &Agent::cell() start");L("Cell &Agent::cell() end");
        return data.cell;
    }

    Agent::Agent(Agent_type type) {
        L("Agent::Agent(Agent_type ) start");
        data.type = std::move(type);
        data.color = Black;
        status = Started;
        L("Agent::Agent(Agent_type ) end");
    }

    void Agent::set_status(Agent_status s) {
        L("Agent::set_status(Agent_status ) start");
        status = s;
        L("Agent::set_status(Agent_status ) end");
    }

    void Agent::set_color(Color color) {
        L("Agent::set_color(Color ) start");
        data.color = color;L("Agent::set_color(Color ) end");
    }

    int32_t State::find(const std::string &type_name) const {
        L("State::find(const std::string &) start");
        for (uint32_t i = 0; i < agents_data.size(); i++) if (agents_data[i].type.name == type_name) return i;L(
                "State::find(const std::string &) end");
        return Not_found;
    }

     Stochastic_move::Stochastic_move(const Connection_pattern &d, vector<uint32_t> c) :
            destinations(d),
            chances(c) {

    }

    Coordinates Stochastic_move::get_move() {
        L("Coordinates Stochastic_agent_action::get_destination() start");
        uint32_t i = Chance::pick(chances);L(i);
        Coordinates c = destinations[i];L(c);L("Coordinates Stochastic_agent_action::get_destination() end");
        return c;
    }
}