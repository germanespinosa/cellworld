#include <cell_world/capture.h>

namespace cell_world {

    Capture::Capture(const Capture_parameters &parameters, const World &world):
            dist_max (parameters.distance * world.cell_transformation.size),
            theta_diff_max(to_radians(parameters.angle / 2)),
            visibility(Location_visibility(world.create_cell_group() ,world.cell_shape, world.cell_transformation)){
    }

    bool Capture::is_captured(const Location &predator_location, double predator_theta, const Location &prey_location) {
        if (predator_location.dist(prey_location) > dist_max) return false;
        if (angle_difference(predator_location.atan(prey_location),predator_theta) > theta_diff_max) return false;
        return visibility.is_visible(predator_location, prey_location) ;
    }
}