from json_cpp import JsonObject, JsonList
from .util import *
from .location import Location, Location_list
from .coordinates import Coordinates, Coordinates_list
from .shape import Shape, Transformation, Space, Transformation_list
from .cell import Cell, Cell_group_builder, Cell_group, Cell_map
from .util import check_type


class World_configuration(JsonObject):
    def __init__(self):
        self.cell_shape = Shape()
        self.cell_coordinates = Coordinates_list()
        self.connection_pattern = Coordinates_list()

    @staticmethod
    def get_from_name(name):
        if not type(name) is str:
            raise "incorrect type for name"
        return World_configuration.parse(json_dictionary=get_resource("world_configuration", name))


class World_implementation(JsonObject):

    def __init__(self, cell_locations=None, space=None, cell_transformation=None):
        if cell_locations is None:
            cell_locations = Location_list()
        if space is None:
            space = Space()
        if cell_transformation is None:
            cell_transformation = Transformation()
        self.cell_locations = cell_locations
        self.space = space
        self.cell_transformation = cell_transformation

    @staticmethod
    def get_from_name(world_configuration_name, world_implementation_name):
        if not type(world_configuration_name) is str:
            raise "incorrect type for world_configuration_name"
        if not type(world_implementation_name) is str:
            raise "incorrect type for world_implementation_name"
        return World_implementation.parse(json_dictionary=get_resource("world_implementation", world_configuration_name, world_implementation_name))

    @staticmethod
    def create(world_configuration, space, cell_transformation=None, relative_locations=None, relative_locations_transformations=None, center_coordinates=None):
        if center_coordinates is None:
            center_coordinates = Coordinates(0, 0)
        if relative_locations is None:
            if relative_locations_transformations is None:
                raise "either relative_locations or relative_locations_transformations must be used"
            check_type(relative_locations_transformations,Transformation_list, "wrong type for relative_locations_transformations")
            relative_locations = World_implementation.create_cell_locations(relative_locations_transformations)
            if len(relative_locations) != len(world_configuration.connection_pattern):
                raise "number of transformations must match the number of connections in the connection pattern"

        check_type(world_configuration, World_configuration, "incorrect type for world_configuration")
        check_type(space, Space, "incorrect type for space")
        check_type(center_coordinates, Coordinates, "incorrect type for center_coordinates")
        check_type(relative_locations, Location_list, "incorrect type for relative_locations")
        if len(world_configuration.connection_pattern) != len(relative_locations):
            raise "the number of locations must match the number of connections"
        wi = World_implementation(space=space, cell_transformation=cell_transformation)
        for x in world_configuration.cell_coordinates:
            wi.cell_locations.append(Location())
        completed = [False for x in world_configuration.cell_coordinates]
        cmap = Cell_map(world_configuration.cell_coordinates)
        wi.cell_locations[cmap[center_coordinates]] = space.center
        set_location(wi.cell_locations, world_configuration.connection_pattern, relative_locations, cmap, center_coordinates, completed)
        return wi

    @staticmethod
    def create_cell_locations(transformations):
        check_type(transformations, list, "incorrect type for transformations")
        cell_locations = Location_list()
        for transformation in transformations:
            check_type(transformation, Transformation, "incorrect type for transformation")
            theta = math.radians(transformation.rotation)
            cell_location = Location(0, 0)
            cell_location.move(theta, transformation.size)
            cell_locations.append(cell_location)
        return cell_locations

    def transform (self, dst_space, cell_transformation=None):
        check_type(dst_space, Space, "incorrect type for dst_space")
        for index, location in enumerate(self.cell_locations):
            new_location = Space.transform_to(location, self.space, dst_space)
            self.cell_locations[index] = new_location
        if cell_transformation is None:
            cell_transformation = Transformation()
            cell_transformation.size = self.cell_transformation.size * dst_space.transformation.size / self.space.transformation.size
            cell_transformation.rotation = self.cell_transformation.rotation + dst_space.transformation.rotation - self.space.transformation.rotation
        check_type(cell_transformation, Transformation, "incorrect type for cell_transformation")
        self.space = dst_space
        self.cell_transformation = cell_transformation


class World:
    def __init__(self, world_configuration):
        if not isinstance(world_configuration, World_configuration):
            raise "incorrect type for world_configuration"
        self.configuration = world_configuration
        self.implementation = None
        self.cells = Cell_group()
        for cc in world_configuration.cell_coordinates:
            self.cells.append(Cell(cell_id=len(self.cells), coordinates=cc))

    def set_implementation(self, world_implementation):
        self.implementation = world_implementation
        for cell_id, location in enumerate(world_implementation.cell_locations):
            self.cells[cell_id].location = location

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
        if world_implementation:
            check_type(world_implementation, World_implementation, "incorrect type for world_implementation")
            w.set_implementation(world_implementation)

        if occlusions:
            check_type(occlusions, Cell_group_builder, "incorrect type for occlusions")
            for cell_id in occlusions:
                w.cells[cell_id].occluded = True
        return w

    def __str__(self):
        return str(self.cells)


def set_location(location_list, connection_pattern, relative_locations, cmap, coordinates, completed):
    check_type(location_list, Location_list, "incorrect type for location_list")
    check_type(connection_pattern, Coordinates_list, "incorrect type for connection_pattern")
    check_type(relative_locations, Location_list, "incorrect type for relative_locations")
    check_type(cmap, Cell_map, "incorrect type for map")
    check_type(coordinates, Coordinates, "incorrect type for coordinates")
    index = cmap[coordinates]
    if completed[index]:
        return
    completed[index] = True
    location = location_list[index]
    for i, c in enumerate(connection_pattern):
        cc = c + coordinates
        cindex = cmap[cc]
        if cindex >= 0:
            location_list[cindex] = location + relative_locations[i]
            set_location(location_list, connection_pattern, relative_locations, cmap, cc, completed)


