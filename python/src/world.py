from json_cpp import JsonObject, JsonList
from .util import *
from .location import Location, Location_list
from .coordinates import Coordinates, Coordinates_list
from .shape import Shape, Transformation, Space, Transformation_list
from .cell import Cell, Cell_group_builder, Cell_group, Cell_map


class World_info(JsonObject):

    def __init__(self, world_configuration: str = "", world_implementation: str = "", occlusions: str = ""):
        self.world_configuration = world_configuration
        self.world_implementation = world_implementation
        self.occlusions = occlusions


class World_configuration(JsonObject):

    def __init__(self):
        self.cell_shape = Shape()
        self.cell_coordinates = Coordinates_list()
        self.connection_pattern = Coordinates_list()

    @staticmethod
    def get_from_name(name: str):
        if not type(name) is str:
            raise "incorrect type for name"
        return World_configuration.parse(json_dictionary=get_resource("world_configuration", name))


class World_implementation(JsonObject):

    def __init__(self,
                 cell_locations: Location_list = None,
                 space: Space = None,
                 cell_transformation: Transformation = None):
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
    def get_from_name(world_configuration_name: str, world_implementation_name: str):
        if not type(world_configuration_name) is str:
            raise "incorrect type for world_configuration_name"
        if not type(world_implementation_name) is str:
            raise "incorrect type for world_implementation_name"
        return World_implementation.parse(json_dictionary=get_resource("world_implementation", world_configuration_name, world_implementation_name))

    @staticmethod
    def create(world_configuration: World_configuration,
               space: Space,
               cell_transformation: Transformation = None,
               relative_locations: Location_list = None,
               relative_locations_transformations: Transformation_list = None,
               center_coordinates: Location_list = None):
        if center_coordinates is None:
            center_coordinates = Coordinates(0, 0)
        if relative_locations is None:
            if relative_locations_transformations is None:
                raise "either relative_locations or relative_locations_transformations must be used"
            relative_locations = World_implementation.create_cell_locations(relative_locations_transformations)
            if len(relative_locations) != len(world_configuration.connection_pattern):
                raise "number of transformations must match the number of connections in the connection pattern"

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
    def create_cell_locations(transformations: Transformation_list):
        cell_locations = Location_list()
        for transformation in transformations:
            theta = math.radians(transformation.rotation)
            cell_location = Location(0, 0)
            cell_location.move(theta, transformation.size)
            cell_locations.append(cell_location)
        return cell_locations

    def transform (self,
                   dst_space: Space,
                   cell_transformation: Transformation=None):
        for index, location in enumerate(self.cell_locations):
            new_location = Space.transform_to(location, self.space, dst_space)
            self.cell_locations[index] = new_location
        if cell_transformation is None:
            cell_transformation = Transformation()
            cell_transformation.size = self.cell_transformation.size * dst_space.transformation.size / self.space.transformation.size
            cell_transformation.rotation = self.cell_transformation.rotation + dst_space.transformation.rotation - self.space.transformation.rotation
        self.space = dst_space
        self.cell_transformation = cell_transformation


class World:
    def __init__(self, world_configuration: World_configuration):
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
    def get_from_parameters_names(
            world_configuration_name: str,
            world_implementation_name: str,
            occlussions_name: str = None):
        wc = World_configuration.get_from_name(world_configuration_name)
        o = None
        wi = World_implementation.get_from_name(world_configuration_name, world_implementation_name)
        if occlussions_name:
            o = Cell_group_builder.get_from_name(world_configuration_name, occlussions_name, "occlusions")
        return World.get_from_parameters(wc, wi, o)

    @staticmethod
    def get_from_parameters(
            world_configuration: World_configuration,
            world_implementation: World_implementation = None,
            occlusions: Cell_group_builder = None):
        wc = world_configuration
        w = World(wc)
        if world_implementation:
            w.set_implementation(world_implementation)

        if occlusions:
            for cell_id in occlusions:
                w.cells[cell_id].occluded = True
        return w

    def __str__(self):
        return str(self.cells)


def set_location(
        location_list: Location_list,
        connection_pattern: Coordinates_list,
        relative_locations: Location_list,
        cmap: Cell_map,
        coordinates: Coordinates,
        completed):
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


