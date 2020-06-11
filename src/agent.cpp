#include <agent.h>
#include <utility>
#include <mutex>

using namespace std;
namespace cell_world {

    const Cell &Agent::cell() const {
        return data.cell;
    }

    Agent::Agent(Agent_type type) :
    status(Update_pending),
    _agent_index(0),
    _message_group(0){
        data.type = std::move(type);
        data.color = Black;
        Agent_broadcaster::add(this, _message_group);
        set_value(0);
    }

    void Agent::set_status(Agent_status s) {
        status = s;
    }

    void Agent::set_color(Color color) {
        data.color = color;
    }

    void Agent::receive_message(const Agent_message &) {

    }

    void Agent::send_message(const Agent_type &to, const std::string &body) const {
        Agent_message m;
        m.from.name = data.type.name;
        m.from.version = data.type.version;
        m.to = to;
        m.body = body;
        Agent_broadcaster::send(m,_message_group);
    }

    void Agent::send_message(const std::string &body) const {
        send_message({},body);
    }

    void Agent::_set_cell(const Cell &cell) {
        data.cell = cell;
    }

    double Agent::get_value() {
        return value;
    }

     int State::find(const std::string &type_name) const {
        for (unsigned int i = 0; i < agents_data.size(); i++) if (agents_data[i].type.name == type_name) return i;
        return Not_found;
    }

     Stochastic_move::Stochastic_move(const Connection_pattern &d, vector<unsigned int> c) :
            destinations(d),
            chances(std::move(c)) {
    }

    Coordinates Stochastic_move::get_move() {
        unsigned int i = Chance::pick(chances);
        return destinations[i];
    }

    static std::vector<std::vector<Agent*>> agent_broadcaster_agents;
    static unsigned int agent_broadcaster_counter = 0;
    static std::mutex agent_broadcaster_mutex;

    void Agent_broadcaster::add(Agent *agent, unsigned int g) {
        agent_broadcaster_mutex.lock();
        while (agent_broadcaster_agents.size() <= g) agent_broadcaster_agents.emplace_back();
        agent_broadcaster_agents[g].push_back(agent);
        agent_broadcaster_mutex.unlock();
    }

    void Agent_broadcaster::send(const Agent_message &m, unsigned int g) {
        if (g>=agent_broadcaster_agents.size()) return;
        for (auto a:agent_broadcaster_agents[g])
            if (( m.to.name.empty() || a->data.type.name == m.to.name ) &&
                ( m.to.version == 0 || a->data.type.version == m.to.version )) a->receive_message(m);
    }

    unsigned int Agent_broadcaster::new_message_group() {
        agent_broadcaster_mutex.lock();
        unsigned int i = ++agent_broadcaster_counter;
        agent_broadcaster_mutex.unlock();
        return i;
    }

    void Agent::set_value(double v) {
        value = v;
    }
}