#include <cell_world/experiment.h>

namespace cell_world{


    size_t Trajectories::get_step_index_by_frame(unsigned int frame, bool exact) {
        size_t b = 0;
        size_t e = size()-1;
        size_t m = (b+e) / 2;

        while (e-b > 1){
            unsigned int f=(*this)[m].frame;
            if ( f == frame){
                return m;
            }
            if ( f > frame){
                b = m + 1;
            }
            if ( f > frame){
                e = m - 1;
            }
            m = (b+e) / 2;
        }
        if ((*this)[m].frame != frame && exact){
            throw std::runtime_error("frame not found");
        }
        return m;
    }
}