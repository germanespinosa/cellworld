#include <agent.h>
#include <utility>

using namespace cell_world;
using namespace std;

const Cell &Agent::cell() const {
    L("Cell &Agent::cell() start");
    L("Cell &Agent::cell() end");
    return data.cell;
}

Agent::Agent(Agent_type type) {
    L("Agent::Agent(Agent_type ) start");
    data.type = std::move(type);
    data.status = Started;
    L("Agent::Agent(Agent_type ) end");
}

void Agent::set_status(Agent_status status) {
    L("Agent::set_status(Agent_status ) start");
    data.status = status;
    L("Agent::set_status(Agent_status ) end");
}

void Agent::set_color(Color color) {
    L("Agent::set_color(Color ) start");
    data.color = color;
    L("Agent::set_color(Color ) end");
}

bool Agent_action::load(const std::string &name) {
    L("Agent_action::load(const std::string &)  start");
    bool res = destinations.load(name) && probabilities.load(name);
    L("Agent_action::load(const std::string &) end");
    return res;
}

bool Agent_action::save(const std::string &name) const {
    L("Agent_action::save(const std::string &) start");
    bool res = destinations.save(name) && probabilities.save(name);
    L("Agent_action::save(const std::string &) end");
    return res;
}

Agent_action::Agent_action(const Connection_pattern &destinations, Chance probabilities)
        : destinations(destinations), probabilities(probabilities) {
    L("Agent_action::Agent_action(const Connection_pattern &, Chance ) start");
    L("Agent_action::Agent_action(const Connection_pattern &, Chance ) end");
}

int32_t State::find(const std::string &type_name) const {
    L("State::find(const std::string &) start");
    for (uint32_t i = 0; i < agents_data.size(); i++) if (agents_data[i].type.name == type_name) return i;
    L("State::find(const std::string &) end");
    return Not_found;
}
