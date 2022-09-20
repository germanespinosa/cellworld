#include <cell_world/capture.h>
#include <cell_world/geometry.h>

namespace cell_world {

    Capture::Capture(const Capture_parameters &parameters, const World &world):
            dist_max (parameters.distance * world.cell_transformation.size),
            theta_diff_max(to_radians(parameters.angle / 2)),
            cool_down(parameters.cool_down),
            visibility(world.create_cell_group() ,world.cell_shape, world.cell_transformation){
    }

    bool Capture::is_captured(const Location &predator_location, float predator_theta, const Location &prey_location) {
        if (!cool_down.time_out()) return false;
        if (predator_location.dist(prey_location) > dist_max) return false;
        if (angle_difference(predator_location.atan(prey_location),predator_theta) > theta_diff_max) return false;
        if (visibility.is_visible(predator_location, prey_location)) {
            cool_down.reset();
            return true;
        } else {
            return false;
        }
    }

    bool Cell_capture::is_captured (const Cell &predator_cell, const Cell &prey_cell) {
        if (predator_cell.coordinates.manhattan(prey_cell.coordinates) > dist_max) return false;
        if (visibility[predator_cell].contains(prey_cell)) return true;
        return false;
    }

    Cell_capture::Cell_capture (const Capture_parameters &parameters, const Graph &visibility):
            visibility(visibility),
            dist_max(parameters.distance * 2){
    }
}