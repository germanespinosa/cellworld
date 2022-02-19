#include <cell_world/peeking.h>

namespace cell_world {
    Peeking::Peeking(const Peeking_parameters &parameters, const World &world):
            peeking_visibility(world.create_cell_group() ,world.cell_shape, Transformation((1.0 + parameters.margin) * world.cell_transformation.size,world.cell_transformation.rotation)),
            peeking_timer(parameters.time_limit) {

    }

    void Peeking::not_visible() {
        peeking_timer.reset();
    }

    bool Peeking::is_seen(const Location &predator_location, const Location &prey_location) {
        if (peeking_visibility.is_visible(predator_location, prey_location)){
            return true;
        }
        if (peeking_timer.time_out()) {
            return true;
        }
        return false;
    }
}