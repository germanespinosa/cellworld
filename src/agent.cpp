#include <agent.h>
#include <utility>
#include <mutex>

using namespace std;
namespace cell_world {

    const Cell &Agent::cell() const {
        L("Cell &Agent::cell() start");L("Cell &Agent::cell() end");
        return data.cell;
    }

    Agent::Agent(Agent_type type) :
    status(Started),
    _message_group(0){
        L("Agent::Agent(Agent_type ) start");
        data.type = std::move(type);
        data.color = Black;
        Agent_broadcaster::add(this, _message_group);
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

    void Agent::receive_message(const Agent_message &) {

    }

    void Agent::send_message(const Agent_type &to, const std::string &body) {
        Agent_message m;
        m.from.name = data.type.name;
        m.from.version = data.type.version;
        m.to = to;
        m.body = body;
        Agent_broadcaster::send(m,_message_group);
    }

    void Agent::send_message(const std::string &body) {
        send_message({},body);
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

    static std::vector<std::vector<Agent*>> agent_broadcaster_agents;
    static uint32_t agent_broadcaster_counter = 0;
    static std::mutex agent_broadcaster_mutex;

    void Agent_broadcaster::add(Agent *agent, uint32_t g) {
        agent_broadcaster_mutex.lock();
        while (agent_broadcaster_agents.size() <= g) agent_broadcaster_agents.emplace_back();
        agent_broadcaster_agents[g].push_back(agent);
        agent_broadcaster_mutex.unlock();
    }

    void Agent_broadcaster::send(const Agent_message &m, uint32_t g) {
        if (g>=agent_broadcaster_agents.size()) return;
        for (auto a:agent_broadcaster_agents[g])
            if (( m.to.name.empty() || a->data.type.name == m.to.name ) &&
                ( m.to.version == 0 || a->data.type.version == m.to.version )) a->receive_message(m);
    }


    uint32_t Agent_broadcaster::new_message_group() {
        agent_broadcaster_mutex.lock();
        uint32_t i = ++agent_broadcaster_counter;
        agent_broadcaster_mutex.unlock();
        return i;
    }
}