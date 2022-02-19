from json_cpp import JsonObject
from .world import World
from .shape import Polygon_list
from .visibility import to_radians, angle_difference, Location_visibility
from .location import Location
from .util import Timer


class Capture_parameters(JsonObject):

    def __init__(self, distance: float = 0.0, angle: float = 0.0, cool_down: float = 0.0):
        self.distance = distance
        self.angle = angle
        self.cool_down = cool_down


class Capture:

    def __init__(self, parameters: Capture_parameters, world: World):
        self.dist_max = parameters.distance * world.implementation.cell_transformation.size
        self.theta_diff_max = to_radians(parameters.angle / 2)
        occlusions_locations = world.cells.occluded_cells().get("location")
        occlusions_polygons = Polygon_list.get_polygons(occlusions_locations, world.configuration.cell_shape.sides, world.implementation.cell_transformation.size / 2, world.implementation.space.transformation.rotation + world.implementation.cell_transformation.rotation)
        self.visibility = Location_visibility(occlusions_polygons)
        self.cool_down = Timer(parameters.cool_down)

    def is_captured(self, predator_location: Location, predator_theta : float, prey_location: Location) -> bool:
        if self.cool_down:
            return False
        if predator_location.dist(prey_location) > self.dist_max:
            return False
        theta = predator_location.atan(prey_location)
        theta_diff, direction = angle_difference(predator_theta, theta)
        if theta_diff > self.theta_diff_max:
            return False
        if self.visibility.is_visible(predator_location, prey_location):
            self.cool_down.reset()
            return True
        else:
            return False
