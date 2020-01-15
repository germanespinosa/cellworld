#include <model.h>
#include <ge211.h>
#include <iostream>

using namespace std;
using namespace cell_world;

void Model::_epoch(){
    state.iteration++;
    for (Agent * _agent : _agents) {
        auto action = _agent->get_action();
        auto move = action.possible_actions[action.probabilities.pick()];
        int32_t destination_index = _cell_group.find(_agent->data.cell.coordinates+move);
        if ( destination_index!= Not_found && !_world[destination_index].occluded )
            _agent->data.cell = _cell_group[destination_index];
    }
    for (auto & _agent : _agents) _agent->update_state(state);
}

bool Model::update() // if all agents made their moves, it triggers an new epoch
{
    bool finish = true;
    bool epoch_ready = true; // assumes no new actions
    for (auto & _agent : _agents){ // ask all agents to make their moves
        finish = finish && !_agent->active ; // check if all agents are done
        if ( _agent->action_ready() != state.iteration ) { //ask if the agents decided what to do
            epoch_ready = false; //one agent has not made its mind
        }
    }
    if (finish) return false; //if all agents are down, terminates the simulation
    if (epoch_ready) _epoch(); //if all agents are done with their actions, trigger the epoch
    return true;
}

vector<Agent_data> Model::get_agents_data(){
    vector<Agent_data> r;
    for (auto & _agent : _agents) r.push_back(_agent->data);
    return r;
}

Model::Model( World &world, std::vector<Agent*> &agents ) : 
    _world (world),
    _agents (agents),
    _cell_group(world.create_cell_group())
    {}

void Model::end_episode() {
    for(auto & _agent : _agents) _agent->end_episode(state);
}

void Model::start_episode() {
    state.iteration = 0;
    for(unsigned int i=0;i<_agents.size();i++) {
        _agents[i]->data.id = i;
        state.agents_data.push_back(_agents[i]->data);
    }
    for(auto & _agent : _agents) _agent->start_episode(state);
}
