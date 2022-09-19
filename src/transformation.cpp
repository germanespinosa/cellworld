#include <cell_world/transformation.h>
#include <cell_world/geometry.h>

namespace cell_world {
    float Transformation::theta() const {
        return to_radians(rotation);
    }

    cell_world::Transformation::Transformation() = default;

    cell_world::Transformation::Transformation(float size, float rotation):
            size(size), rotation(rotation){

    }
}