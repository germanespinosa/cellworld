#include <cell_world/model.h>
#include <iostream>

using namespace std;

namespace cell_world {

    bool Model::update()
    {
        if (state.status != Model_state::Status::Running) throw logic_error("Model::update - model is not running.");
        Agent &agent = _agents[state.current_turn];
        auto &agent_state = state.agents_state[state.current_turn];
        agent_state.iteration++;
        auto move = agent.get_move(state);
        auto &agent_cell = agent_state.cell;
        int destination_index = map.find(agent_cell.coordinates + move);
        state.current_turn++;
        if (destination_index != Not_found) agent_cell = map.cells[destination_index];
        if (state.current_turn == _agents.size()) state.current_turn = 0;
        auto agent_status = agent.update_state(state);
        return state.agents_state[state.current_turn].iteration < state.iterations &&
               agent_status == Agent_status_code::Running;
    }

    Model::Model(Cell_group &cells, unsigned int iterations) :
            map(cells.free_cells()),
            visibility(Visibility::create_graph(cells)){
        state.status = Model_state::Status::Idle;
        state.iterations = iterations;
    }

    Model::Model(Cell_group &cg) : Model(cg, 50) {}

    void Model::end_episode() {
        if (state.status != Model_state::Status::Running) throw logic_error("Model::end_episode - model is not running.");
        state.status = Model_state::Status::Stopped;
        for (Agent &agent:_agents) agent.end_episode(state);
    }

    void Model::start_episode() {
        if (state.status == Model_state::Status::Running) throw logic_error("Model::start_episode - model is already running.");
        if (_agents.empty()) throw logic_error("Model::start_episode - can't start an episode with no agents.");
        state.current_turn = 0;
        state.agents_state.clear();
        for (Agent &agent:_agents) {
            Cell c = agent.start_episode();
            if (map.cells.find(c)==Not_found) throw logic_error("Model::start_episode - agent start cell not available.");
            state.agents_state.emplace_back(0,c);
        }
        state.status = Model_state::Status::Running;
        for (Agent &agent:_agents) agent.update_state(state);
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
}