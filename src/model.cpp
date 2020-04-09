#include <model.h>
#include <ge211.h>
#include <iostream>

using namespace std;
using namespace cell_world;

void Model::_epoch(){
    L("Model::_epoch() start");
    iteration++;
    for (Agent * _agent : _agents) {
        auto move = _agent->get_move(); // read the action from the agent
        int32_t destination_index = _map.find( _agent->data.cell.coordinates + move );
        if ( destination_index!= Not_found && !cells[destination_index].occluded ) {
            _agent->data.cell = cells[destination_index];
        }
    }
    for (uint32_t agent_index = 0; agent_index < _agents.size() ; agent_index++) {
        _agents[agent_index]->update_state(get_state(agent_index));
    }
    L("Model::_epoch() end");
}

bool Model::try_update()
{
    L("Model::update() start");
    if (status != Status::Running) throw logic_error("Model::update - model is not running.");
    bool epoch_ready = true; // assumes no new actions
    finished = false;
    for (auto &_agent : _agents) { // ask all agents to make their moves
        epoch_ready = epoch_ready && _agent->status == Action_ready;
        finished = finished || _agent->status == Finished; // check if all agents are done
    }
    if (epoch_ready) _epoch(); //if all agents are done with their actions, trigger the epoch
    finished = finished || iteration >= iterations;
    L("Model::update() end");
    return epoch_ready;
}

bool Model::update() // if all agents made their moves, it triggers an new epoch
{
    if (status != Status::Running) throw logic_error("Model::update - model is not running.");
    L("Model::update() start");
    while (!try_update() && !finished);
    L("Model::update() end");
    return !finished;
}

vector<Agent_data> Model::get_agents_data(){
    L("Model::get_agents_data() start");
    vector<Agent_data> r;
    for (auto & _agent : _agents) r.push_back(_agent->data);
    L("Model::get_agents_data() end");
    return r;
}

Model::Model( Cell_group &cg, uint32_t iterations ) :
    iterations(iterations),
    cells(cg),
    _map(cells),
    _visibility(Visibility::create_graph(cells)),
    _message_group(Agent_broadcaster::new_message_group())
    {
        L("Model::Model( World &, std::vector<Agent*> &) start");
        status = Status::Idle;
        L("Model::Model( World &, std::vector<Agent*> &) end");
    }

Model::Model( Cell_group &cg ) : Model(cg,50) {}

void Model::end_episode() {
    L("Model::end_episode() start");
    if (status != Status::Running) throw logic_error("Model::end_episode - model is not running.");
    State state = get_state();
    for(auto & _agent : _agents) _agent->end_episode(state);
    status = Status::Stopped;
    L("Model::end_episode() end");
}

void Model::start_episode(uint32_t initial_iteration) {
    if (status == Status::Running) throw logic_error("Model::start_episode - model is already running.");
    L("Model::start_episode() start");
    iteration = initial_iteration;
    for(auto & _agent : _agents) {
        _agent->status = Started;
        auto c = _agent->start_episode(initial_iteration);
        _agent->data.cell = c;
    }
    for (uint32_t agent_index = 0; agent_index < _agents.size() ; agent_index++) {
        _agents[agent_index]->update_state(get_state(agent_index));
    }
    status = Status::Running;
    L("Model::start_episode() end");
}

State Model::get_state() {
    L("Model::get_state() start");
    State state;
    L("Model::get_state() - state.iteration = iteration");
    state.iteration = iteration;
    L("Model::get_state() - for(auto & _agent : _agents)");
    for(auto & _agent : _agents) {
        L("Model::get_state() - state.agents_data.push_back(_agent->data);");
        state.agents_data.push_back(_agent->data);
    }
    L("Model::get_state() end");
    return state;
}

State Model::get_state(uint32_t agent_index) {
    L("Model::get_state(uint32_t) start");
    L("Model::get_state(uint32_t) - auto cell = _agents[agent_index]->data.cell");
    auto cell = _agents[agent_index]->data.cell;
    L("Model::get_state(uint32_t) - auto vi = _visibility[cell];");
    auto vi = _visibility[cell];
    State state;
    state.iteration = iteration;
    state.iterations = iterations;
    L("Model::get_state(uint32_t) - for (uint32_t index = 0; index < _agents.size() ; index++)");
    for (uint32_t index = 0; index < _agents.size() ; index++) {
        L("Model::get_state(uint32_t) - if (index != agent_index && vi.contains(_agents[index]->data.cell))");
        if (index != agent_index && vi.contains(_agents[index]->data.cell)) {
            L("Model::get_state(uint32_t) - state.agents_data.push_back(_agents[index]->data);");
            state.agents_data.push_back(_agents[index]->data);
        }
        L("Model::get_state(uint32_t) - end if");
    }
    L("Model::get_state(uint32_t) end");
    return state;
}

void Model::add_agent(Agent &agent) {
    _agents.push_back(&agent);
    // agents attached to the model can only send messages to other agents in the model
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
    start_episode(0);
}
