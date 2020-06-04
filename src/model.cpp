#include <model.h>
#include <ge211.h>
#include <iostream>

using namespace std;

namespace cell_world {

    bool Model::try_update() {
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
        finished = finished || iteration >= iterations;
        return !finished;
    }

    vector<Agent_data> Model::get_agents_data() {
        vector<Agent_data> r;
        for (auto &_agent : _agents) r.push_back(_agent.get().data);
        return r;
    }

    Model::Model(Cell_group &cg, uint32_t iterations) :
            iteration(0),
            iterations(iterations),
            cells(cg),
            mode(Mode::Turns),
            map(cells),
            visibility(Visibility::create_graph(cells)),
            log(iterations),
            _message_group(Agent_broadcaster::new_message_group()),
            _current_turn(0) {
        status = Status::Idle;
        state.iterations = iterations;
    }

    Model::Model(Cell_group &cg) : Model(cg, 50) {}

    void Model::end_episode() {
        if (status != Status::Running) throw logic_error("Model::end_episode - model is not running.");
        State state = get_state();
        for (auto &_agent : _agents) _agent.get().end_episode(state);
        status = Status::Stopped;
    }

    void Model::start_episode(uint32_t initial_iteration) {
        if (status == Status::Running) throw logic_error("Model::start_episode - model is already running.");
        if (_agents.empty()) throw logic_error("Model::start_episode - can't start an episode with no agents.");
        iteration = initial_iteration;
        _current_turn = 0;
        finished = false;
        for (uint32_t i = 0; i < _agents.size(); i++) {
            auto &agent = _agents[i].get();
            agent.status = Update_pending;
            Cell c = agent.start_episode(initial_iteration);
            log.start_coordinates(i, c.coordinates);
            agent._set_cell(c);
        }
        status = Status::Running;
    }

    State &Model::get_state() {
        state.iteration = iteration;
        for (uint32_t a = 0; a<_agents.size(); a++) {
            state.visible[a] = false;
            state.agents_data[a] = _agents[a].get().data;
        }
        return state;
    }

    State &Model::get_state(uint32_t agent_index) {
        auto cell = _agents[agent_index].get().data.cell;
        auto vi = visibility[cell];
        state.iteration = iteration;
        for (uint32_t index = 0; index < _agents.size(); index++) {
            state.visible[index] = vi.contains(_agents[index].get().data.cell);
            state.agents_data[index] = _agents[index].get().data;
        }
        return state;
    }

    void Model::add_agent(Agent &agent) {
        agent._agent_index = _agents.size();
        _agents.emplace_back(agent);
        // agents attached to the model can only send messages to other agents in the model
        agent._message_group = _message_group;
        Agent_broadcaster::add(&agent, _message_group);
        log.add_agent();
        state.visible.emplace_back();
        state.agents_data.emplace_back();
    }

    void Model::run() {
        run(iterations);
    }

    void Model::run(uint32_t to_iteration) {
        for (; iteration < to_iteration && update(););
    }

    void Model::start_episode() {
        finished = false;
        start_episode(0);
    }

    bool Model::_try_update_simultaneous() {
        bool epoch_ready = true;
        for (uint32_t agent_index = 0; agent_index < _agents.size(); agent_index++) {
            auto &agent = _agents[agent_index].get();
            if (agent.status == Update_pending) {
                agent.status = Action_pending;
                agent.update_state(get_state(agent_index));
            }
            epoch_ready = epoch_ready && agent.status == Action_ready;
            finished = finished || agent.status == Finished; // check if all agents are done
        }
        if (epoch_ready) {
            for (uint32_t i = 0; i < _agents.size(); i++) {
                auto &agent = _agents[i].get();
                log.set_value(iteration, i, agent.get_value());
                auto move = agent.get_move(); // read the action from the agent
                agent.status = Update_pending;
                int32_t destination_index = map.find(agent.data.cell.coordinates + move);
                if (destination_index != Not_found && !cells[destination_index].occluded) {
                    agent._set_cell(cells[destination_index]);
                }
                log.set_coordinates(iteration, i, agent.data.cell.coordinates);
            }
            iteration++;
        };
        return epoch_ready;
    }

    bool Model::_try_update_turn() {
        bool epoch_ready = false;
        auto &agent = _agents[_current_turn].get();
        if (agent.status == Update_pending) {
            agent.status = Action_pending;
            agent.update_state(get_state(_current_turn));
        }
        if (agent.status == Action_ready) {
            log.set_value(iteration, _current_turn, agent.get_value());
            auto move = agent.get_move();
            agent.status = Update_pending;
            int32_t destination_index = map.find(agent.data.cell.coordinates + move);
            if (destination_index != Not_found && !cells[destination_index].occluded) {
                agent._set_cell(cells[destination_index]);
            }
            log.set_coordinates(iteration, _current_turn, agent.data.cell.coordinates);
            _current_turn++;
            if (_current_turn == _agents.size()) {
                iteration++;
                _current_turn = 0;
                epoch_ready = true;
            }
        };
        finished = finished || (agent.status == Finished);
        return epoch_ready;
    }

    Execution_log::Execution_log(uint32_t iterations) :
            iterations(iterations) {}

    void Execution_log::add_agent() {
        trajectories.emplace_back(iterations + 2);
        values.emplace_back(iterations + 1);
        _last_trajectory.push_back(1);
        _last_value.push_back(0);
    }

    void Execution_log::set_value(uint32_t iteration, uint32_t agent_ind, double value) {
        values[agent_ind][iteration] = value;
        _last_value[agent_ind] = iteration + 1;
    }

    void Execution_log::set_coordinates(uint32_t iteration, uint32_t agent_ind, Coordinates coordinate) {
        trajectories[agent_ind][iteration + 1] = coordinate;
        _last_trajectory[agent_ind] = iteration + 1;
    }

    void Execution_log::start_coordinates(uint32_t agent_ind, Coordinates coordinate) {
        trajectories[agent_ind][0] = coordinate;
    }

    std::ostream &operator<<(std::ostream &o, Execution_log const &log) {
        o << "{ \"trajectories\": [";
        for (uint32_t agent_ind = 0; agent_ind < log.trajectories.size(); agent_ind++) {
            if (agent_ind) o << ",";
            o << "[";
            for (uint32_t iteration = 0; iteration < log._last_trajectory[agent_ind]; iteration++) {
                if (iteration) o << ",";
                o << "[" << (int16_t) log.trajectories[agent_ind][iteration].x << ","
                  << (int16_t) log.trajectories[agent_ind][iteration].y << "]";
            }
            o << "]";
        }
        o << "], \"values\": [";
        for (uint32_t agent_ind = 0; agent_ind < log.values.size(); agent_ind++) {
            if (agent_ind) o << ",";
            o << "[";
            for (uint32_t iteration = 0; iteration < log._last_value[agent_ind]; iteration++) {
                if (iteration) o << ",";
                o << log.values[agent_ind][iteration];
            }
            o << "]";
        }
        o << "]}" << endl;
        return o;
    }
}