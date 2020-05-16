#include <model.h>
#include <ge211.h>
#include <iostream>

using namespace std;
using namespace cell_world;

bool Model::try_update()
{
    if (status != Status::Running) throw logic_error("Model::update - model is not running.");

    if (mode == Mode::Turns) {
        return _try_update_turn();
    } else {
        return _try_update_simultaneous();
    }
}

bool Model::update() // if all agents made their moves, it triggers an new epoch
{
    if (status != Status::Running) throw logic_error("Model::update - model is not running.");
    while (!try_update() && !finished);
    return !finished;
}

vector<Agent_data> Model::get_agents_data(){
    vector<Agent_data> r;
    for (auto & _agent : _agents) r.push_back(_agent.get().data);
    return r;
}

Model::Model( Cell_group &cg, uint32_t iterations ) :
    iteration(0),
    iterations(iterations),
    cells(cg),
    mode(Mode::Turns),
    map(cells),
    visibility(Visibility::create_graph(cells)),
    _message_group(Agent_broadcaster::new_message_group()),
    _current_turn(0)
    {
        status = Status::Idle;
    }

Model::Model( Cell_group &cg ) : Model(cg,50) {}

void Model::end_episode() {
    if (status != Status::Running) throw logic_error("Model::end_episode - model is not running.");
    State state = get_state();
    for(auto & _agent : _agents) _agent.get().end_episode(state, history);
    status = Status::Stopped;
}

void Model::start_episode(uint32_t initial_iteration) {
    if (status == Status::Running) throw logic_error("Model::start_episode - model is already running.");
    if (_agents.empty()) throw logic_error("Model::start_episode - can't start an episode with no agents.");
    iteration = initial_iteration;
    _current_turn = 0;
    finished = false;
    history.clear();
    for(uint32_t i=0;i<_agents.size();i++) {
        auto &agent = _agents[i].get();
        agent.status = Update_pending;
        Cell c = agent.start_episode(initial_iteration);
        agent._set_cell(c);
        history._history[i].push_back(c.coordinates);
    }
    status = Status::Running;
}

State Model::get_state() {
    State state;
    state.iteration = iteration;
    for(auto & _agent : _agents) {
        state.agents_data.push_back(_agent.get().data);
    }
    return state;
}

State Model::get_state(uint32_t agent_index) {
    auto cell = _agents[agent_index].get().data.cell;
    auto vi = visibility[cell];
    State state;
    state.iteration = iteration;
    state.iterations = iterations;
    for (uint32_t index = 0; index < _agents.size() ; index++) {
        if (index != agent_index && vi.contains(_agents[index].get().data.cell)) {
            state.agents_data.push_back(_agents[index].get().data);
        }
    }
    return state;
}

void Model::add_agent(Agent &agent) {
    agent._agent_index = _agents.size();
    _agents.emplace_back(agent);
    // agents attached to the model can only send messages to other agents in the model
    history._history.emplace_back();
    agent._message_group = _message_group;
    Agent_broadcaster::add(&agent, _message_group);
}

void Model::run() {
    run(iterations);
}

void Model::run(uint32_t to_iteration) {
    for(;iteration < to_iteration && update(););
}

void Model::start_episode() {
    finished = false;
    start_episode(0);
}

bool Model::_try_update_simultaneous() {
    bool epoch_ready = true;
    for (uint32_t agent_index = 0; agent_index < _agents.size() ; agent_index++) {
        auto &agent = _agents[agent_index].get();
        if (agent.status == Update_pending) {
            agent.status = Action_pending;
            agent.update_state(get_state(agent_index));
        }
        epoch_ready = epoch_ready && agent.status == Action_ready;
        finished = finished || agent.status == Finished; // check if all agents are done
    }
    if (epoch_ready) {
        for (uint32_t i=0;i<_agents.size();i++) {
            auto &agent = _agents[i].get();
            auto move = agent.get_move(); // read the action from the agent
            agent.status = Update_pending;
            int32_t destination_index = map.find( agent.data.cell.coordinates + move );
            if ( destination_index != Not_found && !cells[destination_index].occluded ) {
                agent._set_cell(cells[destination_index]);
            }
            history._history[i].push_back(agent.data.cell.coordinates);
        }
        iteration++;
        finished = finished || iteration >= iterations;
    };
    return epoch_ready;
}

bool Model::_try_update_turn() {
    bool epoch_ready = false;
    auto &agent = _agents[_current_turn].get();
    if (agent.status == Update_pending){
        agent.status = Action_pending;
        agent.update_state(get_state(_current_turn));
    }
    if (agent.status == Action_ready) {
        auto move = agent.get_move();
        agent.status = Update_pending;
        int32_t destination_index = map.find( agent.data.cell.coordinates + move );
        if ( destination_index!= Not_found && !cells[destination_index].occluded ) {
            agent._set_cell(cells[destination_index]);
        }
        history._history[_current_turn].push_back(agent.data.cell.coordinates);
        _current_turn++;
        if (_current_turn == _agents.size()) {
            iteration++;
            _current_turn = 0;
            epoch_ready = true;
        }
    };
    finished = finished || (agent.status == Finished) || iteration >= iterations;
    return epoch_ready;
}
