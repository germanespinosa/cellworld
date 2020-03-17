#include <model.h>
#include <ge211.h>
#include <iostream>

using namespace std;
using namespace cell_world;

void Model::_epoch(){
    L("Model::_epoch() start");
    iteration++;
    L("Model::_epoch() - for (Agent * _agent : _agents)");
    for (Agent * _agent : _agents) {
        L("Model::_epoch() - auto move = _agent->get_move();");
        auto move = _agent->get_move(); // read the action from the agent
        L("Model::_epoch() - auto _agent->data.status = Action_pending;");
        _agent->data.status = Action_pending; // set the state to pending again
        L("Model::_epoch() - int32_t destination_index = _map.find( _agent->data.cell.coordinates + move );");
        int32_t destination_index = _map.find( _agent->data.cell.coordinates + move );
        L("Model::_epoch() - if ( destination_index!= Not_found && !_world[destination_index].occluded )");
        if ( destination_index!= Not_found && !cells[destination_index].occluded ) {
            L("Model::_epoch() - _agent->data.cell = _cell_group[destination_index];");
            _agent->data.cell = cells[destination_index];
        }
    }
    L("Model::_epoch() - for (uint32_t agent_index = 0; agent_index < _agents.size() ; agent_index++)");
    for (uint32_t agent_index = 0; agent_index < _agents.size() ; agent_index++) {
        L("Model::_epoch() - _agents[agent_index]->update_state(get_state(agent_index));");
        _agents[agent_index]->update_state(get_state(agent_index));
    }
    L("Model::_epoch() end");
}

bool Model::update() // if all agents made their moves, it triggers an new epoch
{
    L("Model::update() start");
    bool finish;
    bool epoch_ready; // assumes no new actions
    do {
        finish = false;
        epoch_ready = true;
        for (auto &_agent : _agents) { // ask all agents to make their moves
            finish = finish || _agent->data.status == Finished; // check if all agents are done
            epoch_ready = epoch_ready && _agent->data.status == Action_ready;
        }
        if (finish) return false; //if any agent is not longer running, terminates the simulation
    } while (!epoch_ready);
    _epoch(); //if all agents are done with their actions, trigger the epoch
    L("Model::update() end");
    return true;
}

vector<Agent_data> Model::get_agents_data(){
    L("Model::get_agents_data() start");
    vector<Agent_data> r;
    for (auto & _agent : _agents) r.push_back(_agent->data);
    L("Model::get_agents_data() end");
    return r;
}

Model::Model( Cell_group &cg ) :
        cells(cg),
    _map(cells),
    _visibility(Visibility::create_graph(cells))
    {
        L("Model::Model( World &, std::vector<Agent*> &) start");

        L("Model::Model( World &, std::vector<Agent*> &) end");
    }

void Model::end_episode() {
    L("Model::end_episode() start");
    State state = get_state();
    for(auto & _agent : _agents) _agent->end_episode(state);
    L("Model::end_episode() end");
}

void Model::start_episode() {
    L("Model::start_episode() start");
    iteration = 0;
    L("Model::start_episode() - for(auto & _agent : _agents)");
    for(auto & _agent : _agents) {
        L("Model::start_episode() - _agent->data.status = Started;");
        _agent->data.status = Started;
        L("Model::start_episode() - _agent->data.cell = _agent->start_episode(state);");
        _agent->data.cell = _agent->start_episode();
    }
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
}

void Model::run(uint32_t iterations) {
    start_episode();
    for (uint32_t i=0;i<iterations && update();i++);
    end_episode();
}
