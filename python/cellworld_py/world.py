import math

from .util import *
from .location import Location, Locations_list
from .coordinates import Coordinates, Coordinates_list
from .shape import Shape, Transformation, Space
from .cell import Cell, Cell_group_builder, Cell_group, Cell_map
from .util import check_type


class World_configuration:
    def __init__(self):
        self.cell_shape = Shape()
        self.cell_coordinates = Coordinates_list()
        self.connection_pattern = Coordinates_list()

    @staticmethod
    def get(world_configuration):
        wc = World_configuration()
        wc.cell_shape = Shape.get(world_configuration["cell_shape"])
        wc.cell_coordinates = Coordinates_list.get(world_configuration["cell_coordinates"])
        wc.connection_pattern = Coordinates_list.get(world_configuration["connection_pattern"])
        return wc

    @staticmethod
    def get_from_name(name):
        if not type(name) is str:
            raise "incorrect type for name"
        return World_configuration.get(get_resource("world_configuration", name))

    def __str__(self):
        return '{"cell_shape":%s,"cell_coordinates":%s,"connection_pattern":%s}' % (self.cell_descriptor, self.cell_coordinates, self.connection_pattern)


class World_implementation(Locations_list):

    def __init__(self):
        self.cell_locations = Locations_list()
        self.space = Space()
        self.cell_transformation = Transformation()

    @staticmethod
    def get(world_implementation):
        wi = World_implementation()
        wi.cell_locations = Locations_list.get(world_implementation["cell_locations"])
        wi.center = Location.get(world_implementation["center"])
        wi.space_shape = Shape.get(world_implementation["space_shape"])
        wi.space_transformation = Transformation.get(world_implementation["space_transformation"])
        wi.cell_transformation = Transformation.get(world_implementation["cell_transformation"])
        return wi

    @staticmethod
    def get_from_name(world_name, name):
        if not type(world_name) is str:
            raise "incorrect type for world_name"
        if not type(name) is str:
            raise "incorrect type for name"
        return World_implementation.get(get_resource("world_implementation", world_name, name))

    @staticmethod
    def create(world_configuration, center_location, center_coordinates, relative_locations=None, relative_locations_transformations=None):
        if relative_locations is None:
            if relative_locations_transformations is None:
                raise "either relative_locations or relative_locations_transformations must be used"
            if type(relative_locations_transformations) is tuple:
                relative_locations_transformations = Transformation.get_transformations(*relative_locations_transformations)
            relative_locations = World_implementation.create_cell_locations(relative_locations_transformations)
            if len(relative_locations.locations) != len(world_configuration.connection_pattern.coordinates):
                raise "number of transformations must match the number of connections in the connection pattern"

        check_type(world_configuration, World_configuration, "incorrect type for world_configuration")
        check_type(center_location, Location, "incorrect type for center_location")
        check_type(center_coordinates, Coordinates, "incorrect type for center_coordinates")
        check_type(relative_locations, Locations_list, "incorrect type for relative_locations")
        if len(world_configuration.connection_pattern.coordinates) != len(relative_locations.locations):
            raise "the number of locations must match the number of connections"
        wi = World_implementation()
        wi.center = center_location
        wi.cell_locations.locations = [Location() for x in range(len(world_configuration.cell_coordinates.coordinates))]
        completed = [False for x in range(len(world_configuration.cell_coordinates.coordinates))]
        cmap = Cell_map(world_configuration.cell_coordinates)
        wi.cell_locations.locations[cmap[center_coordinates]] = center_location
        set_location(wi.cell_locations, world_configuration.connection_pattern, relative_locations, cmap, center_coordinates, completed)
        return wi

    def __str__(self):
        return '{"space":%s,"cell_radius":%f,"cell_locations":%s}' % (str(self.space), self.cell_radius, str(self.cell_descriptor))

    @staticmethod
    def create_cell_locations(transformations):
        check_type(transformations, list, "incorrect type for transformations")
        cell_locations = Locations_list()
        for transformation in transformations:
            check_type(transformation, Transformation, "incorrect type for transformation")
            theta = math.radians(transformation.rotation)
            cell_location = Location(0, 0)
            cell_location.move(theta, transformation.size)
            cell_locations.locations.append(cell_location)
        return cell_locations

    def transform (self, dst_space, cell_transformation=None):
        check_type(dst_space, Space, "incorrect type for dst_space")
        for index, location in enumerate(self.cell_locations):
            new_location = Space.transform_to(location, self.space, dst_space)
            self.cell_locations.locations[index] = new_location
        if cell_transformation is None:
            cell_transformation = Transformation()
            cell_transformation.size = self.cell_transformation.size * dst_space.transformation.size / self.space.transformation.size
            cell_transformation.rotation = self.cell_transformation.rotation + dst_space.transformation.rotation - self.space.transformation.rotation
        check_type(cell_transformation, Transformation, "incorrect type for cell_transformation")
        self.space = dst_space
        self.cell_transformation = cell_transformation


class World:
    def __init__(self, world_configuration=None):
        self.name = ""
        if world_configuration is None:
            self.cell_shape = Shape()
            self.connection_pattern = Coordinates_list()
        else:
            if not isinstance(world_configuration, World_configuration):
                raise "incorrect type for world_configuration"
            self.cell_shape = world_configuration.cell_shape
            self.connection_pattern = world_configuration.connection_pattern
        self.cells = []

    @staticmethod
    def get_from_parameters_names(world_configuration_name, world_implementation_name=None, occlussions_name=None):
        wc = World_configuration.get_from_name(world_configuration_name)
        cl = None
        o = None
        if world_implementation_name:
            wi = World_implementation.get_from_name(world_configuration_name, world_implementation_name)
        if occlussions_name:
            o = Cell_group_builder.get_from_name(world_configuration_name, occlussions_name, "occlusions")
        return World.get_from_parameters(wc, wi, o)

    @staticmethod
    def get_from_parameters(world_configuration, world_implementation=None, occlusions=None):
        if not isinstance(world_configuration, World_configuration):
            raise "incorrect type for world_configuration"

        wc = world_configuration
        w = World(wc)
        w.implementation = world_implementation
        for cell_id, coordinates in enumerate(wc.cell_coordinates.coordinates):
            w.cells.append(Cell(cell_id, coordinates))
        if world_implementation:
            if not isinstance(world_implementation, World_implementation):
                raise "incorrect type for cell_locations"
            for cell_id, location in enumerate(world_implementation.cell_locations.locations):
                w.cells[cell_id].location = location

        if occlusions:
            if not isinstance(occlusions, Cell_group_builder):
                raise "incorrect type for occlusions"

            for cell_id in occlusions.cell_ids:
                w.cells[cell_id].occluded = True
        return w

    @staticmethod
    def get(world):
        w = World()
        w.name = world["name"]
        #w.cell_shape = Shape.get(world["cell_shape"])
        w.connection_pattern = Coordinates_list.get(world["connection_pattern"])
        w.cells = [Cell.get(c) for c in world["cells"]]
        return w

    @staticmethod
    def get_from_name(name):
        if not type(name) is str:
            raise "incorrect type for name"
        return World.get(get_resource("world", name))

    def __str__(self):
        return '{"name":"%s","cell_shape":%s,"cells":[%s],"connection_pattern":%s}' % (self.name, self.cell_descriptor, ",".join([str(cell) for cell in self.cells]), self.connection_pattern)


def set_location(location_list, connection_pattern, relative_locations, cmap, coordinates, completed):
    check_type(location_list, Locations_list, "incorrect type for location_list")
    check_type(connection_pattern, Coordinates_list, "incorrect type for connection_pattern")
    check_type(relative_locations, Locations_list, "incorrect type for relative_locations")
    check_type(cmap, Cell_map, "incorrect type for map")
    check_type(coordinates, Coordinates, "incorrect type for coordinates")
    index = cmap[coordinates]
    if completed[index]:
        return
    completed[index] = True
    location = location_list[index]
    for i, c in enumerate(connection_pattern.coordinates):
        cc = c + coordinates
        cindex = cmap[cc]
        if cindex >= 0:
            location_list.locations[cindex] = location + relative_locations[i]
            set_location(location_list, connection_pattern, relative_locations, cmap, cc, completed)


