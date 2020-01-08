#include <model.h>
#include <ge211.h>
#include <iostream>

using namespace std;
using namespace cellworld;

void Model::_epoch(){
    state.iteration++;
    for (unsigned int i=0;i<_agents.size();i++) {
        uint32_t destination = _agents[i]->get_destination();
        if ( !_world[destination].occluded )
            _agents[i]->data.cell_id = destination;
    }
    for (unsigned int i=0;i<_agents.size();i++) {
        _agents[i]->update_state(state);
    }
}

bool Model::update() // if all agents made their moves, it triggers an new poch
{
    bool finish = true;
    bool epoch_ready = true; // assumes no new actions
    for (unsigned int i=0;i<_agents.size();i++){ // ask all agents to make their moves
        finish = finish && !_agents[i]->active ; // check if all agents are done
        if ( _agents[i]->action_ready() != state.iteration ) { //ask if the agents decided what to do
            epoch_ready = false; //one agent has not made its mind
        }
    }
    if (finish) return false; //if all agents are down, terminates the simulation
    if (epoch_ready) _epoch(); //if all agents are done with their actions, trigger the epoch
    return true;
}

vector<Agent_data> Model::get_agents_data(){
    vector<Agent_data> r;
    for (unsigned int i = 0; i < _agents.size(); i++)
        r.push_back(_agents[i]->data);
    return r;
}

Model::Model( World &world, std::vector<Agent*> &agents ) : 
    _world (world),
    _agents (agents)
{
}

void Model::end_episode() {
    for(unsigned int i=0;i<_agents.size();i++) {
        _agents[i]->end_episode(state);
    }
}

void Model::start_episode() {
    state.iteration = 0;
    for(unsigned int i=0;i<_agents.size();i++) {
        _agents[i]->data.id = i;
        state.agents.push_back(&_agents[i]->data);
    }
    for(unsigned int i=0;i<_agents.size();i++) {
        _agents[i]->start_episode(state);
    }
}
