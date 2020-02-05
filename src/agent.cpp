#include <agent.h>

#include <utility>

const cell_world::Cell &cell_world::Agent::cell() const {
    return data.cell;
}

cell_world::Agent::Agent(cell_world::Agent_type type)
{
    data.type = type;
    data.status = Started;
}

void cell_world::Agent::set_status(cell_world::Agent_status status) {
    data.status = status;
}

void cell_world::Agent::set_color(cell_world::Color color) {
    data.color = color;
}

bool cell_world::Agent_action::load(const std::string &name) {
    return destinations.load(name) && probabilities.load(name);
}

bool cell_world::Agent_action::save(const std::string &name) const {
    return destinations.save(name) && probabilities.save(name);
}

cell_world::Agent_action::Agent_action(cell_world::Connection_pattern destinations, cell_world::Probabilities probabilities)
: destinations(std::move(destinations))
, probabilities(std::move(probabilities)){
}

int32_t cell_world::State::find(const std::string& type_name) const {
    for(uint32_t i = 0; i<agents_data.size() ; i++) if (agents_data[i].type.name == type_name) return i;
    return Not_found;
}
