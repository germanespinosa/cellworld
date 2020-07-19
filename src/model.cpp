#include <cell_world/model.h>
#include <iostream>

using namespace std;

namespace cell_world {

    bool Model::update()
    {
        if (status != Status::Running) throw logic_error("Model::update - model is not running.");
        auto &agent = _agents[_current_turn].get();
        auto move = agent.get_move(state);
        auto &agent_cell = state.agents_state[_current_turn].cell;
        int destination_index = map.find(agent_cell.coordinates + move);
        if (destination_index != Not_found && !cells[destination_index].occluded) {
            agent_cell = cells[destination_index];
        }
        state.agents_state[_current_turn].iteration++;
        auto agent_status = agent.update_state(state);
        _current_turn++;
        if (_current_turn == _agents.size()) {
            _current_turn = 0;
        }
        return state.agents_state[_current_turn].iteration < state.iterations &&
               agent_status == Agent_status_code::Running;
    }

    Model::Model(Cell_group &cg, unsigned int iterations) :
            cells(cg),
            map(cells),
            visibility(Visibility::create_graph(cells)),
            _current_turn(0) {
        status = Status::Idle;
        state.iterations = iterations;
    }

    Model::Model(Cell_group &cg) : Model(cg, 50) {}

    void Model::end_episode() {
        if (status != Status::Running) throw logic_error("Model::end_episode - model is not running.");
        for (unsigned int agent_ind=0;agent_ind<_agents.size();agent_ind++) {
            auto &_agent = _agents[agent_ind].get();
            _agent.end_episode(state);
        }
        status = Status::Stopped;
    }

    void Model::start_episode(unsigned int initial_iteration) {
        if (status == Status::Running) throw logic_error("Model::start_episode - model is already running.");
        if (_agents.empty()) throw logic_error("Model::start_episode - can't start an episode with no agents.");
        _current_turn = 0;
        finished = false;
        state.agents_state.clear();
        for (unsigned int i = 0; i < _agents.size(); i++) {
            auto &agent = _agents[i].get();
            Cell c = agent.start_episode(initial_iteration);
            state.agents_state.emplace_back(initial_iteration,c);
        }
        status = Status::Running;
    }

    void Model::add_agent(Agent &agent) {
        agent._agent_index = _agents.size();
        _agents.emplace_back(agent);
    }

    void Model::run() {
        run(state.iterations);
    }

    void Model::run(unsigned int to_iteration) {
        for (; state.iterations < to_iteration && update(););
    }

    void Model::start_episode() {
        finished = false;
        start_episode(0);
    }
}