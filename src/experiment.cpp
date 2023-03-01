#include <cell_world/experiment.h>

namespace cell_world{


    size_t Trajectories::get_step_index_by_frame(unsigned int frame, bool exact) {
        size_t b = 0;
        size_t e = size()-1;
        size_t m = (b+e) / 2;

        while (e-b > 0){
            unsigned int f=(*this)[m].frame;
            if ( f == frame){
                return m;
            }
            if ( f < frame){
                b = m + 1;
            }
            if ( f > frame){
                e = m - 1;
            }
            size_t nm = (b+e) / 2;
            if (nm == m) break;
            m = nm;
        }
        if ((*this)[m].frame != frame && exact){
            throw std::runtime_error("frame not found");
        }
        return m;
    }

    size_t Trajectories::get_step_index_by_time_stamp(float time_stamp, bool exact) {
        size_t b = 0;
        size_t e = size()-1;
        size_t m = (b+e) / 2;

        while (e-b > 0){
            float f=(*this)[m].time_stamp;
            if ( f == time_stamp){
                return m;
            }
            if ( f < time_stamp){
                b = m + 1;
            }
            if ( f > time_stamp){
                e = m - 1;
            }
            size_t nm = (b+e) / 2;
            if (nm == m) break;
            m = nm;
        }
        if ((*this)[m].time_stamp != time_stamp && exact){
            throw std::runtime_error("time_stamp not found");
        }
        return m;
    }

    Step &Trajectories::get_step_by_frame(unsigned int frame, bool exact) {
        return (*this)[this->get_step_index_by_frame(frame, exact)];
    }

    Step &Trajectories::get_step_by_time_stamp(float time_stamp, bool exact) {
        return (*this)[this->get_step_index_by_time_stamp(time_stamp, exact)];
    }

    Trajectories Trajectories::get_agent_trajectory(const std::string &agent_name) const {
        Trajectories agent_trajectory;
        for(Step s:*this){
            if (s.agent_name == agent_name)
                agent_trajectory.push_back(s);
        }
        return agent_trajectory;
    }
}