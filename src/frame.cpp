#include <cell_world/frame.h>

namespace cell_world {
    Frame_list Frame_list::from_episode(const Episode &episode) {
        unsigned int last_frame = episode.trajectories.max([](const Step &s){return s.frame;});
        Frame_list frame_list(last_frame + 1);
        auto predator_trajectory = episode.trajectories.filter([](const Step &s){return s.agent_name=="predator";});
        auto prey_trajectory = episode.trajectories.filter([](const Step &s){return s.agent_name=="prey";});
        unsigned int frame_number = 0;
        for (auto &frame:frame_list){
            frame.frame_number = frame_number;
            try{
                const Step &prey_step = prey_trajectory.find_first([frame_number](const Step &s){ return s.frame == frame_number;});
                frame.prey_detection = true;
                frame.prey_location = prey_step.location;
                frame.prey_rotation = prey_step.rotation;
                frame.prey_data = prey_step.data;
                frame.time_stamp = prey_step.time_stamp;
            }catch(...){
                frame.prey_detection = false;
            }
            try{
                const Step &predator_step = predator_trajectory.find_first([frame_number](const Step &s){ return s.frame == frame_number;});
                frame.predator_detection = true;
                frame.predator_location = predator_step.location;
                frame.predator_rotation = predator_step.rotation;
                frame.predator_data = predator_step.data;
            }catch(...){
                frame.predator_detection = false;
            }
            frame_number++;
        }
        return frame_list;
    }
}