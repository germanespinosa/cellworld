import random

from json_cpp import JsonObject, JsonList
from .util import *
from .location import Location, Location_list, Scale
from .coordinates import Coordinates, Coordinates_list
from .shape import Shape, Transformation, Space, Transformation_list
from .cell import Cell, Cell_group_builder, Cell_group, Cell_map


def get_occlusion_count(cell_count, target_entropy):
    for c in range(cell_count):
        p = c/cell_count
        if probability_entropy([p, 1-p]) >= target_entropy:
            return c
    return 0


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

    def transform(self,
                   dst_space: Space,
                   cell_transformation: Transformation = None):
        for index, location in enumerate(self.cell_locations):
            new_location = Space.transform_to(location, self.space, dst_space)
            self.cell_locations[index] = new_location
        if cell_transformation is None:
            cell_transformation = self.cell_transformation.copy()
            cell_transformation.size = self.cell_transformation.size * dst_space.transformation.size / self.space.transformation.size
        self.space = dst_space
        self.cell_transformation = cell_transformation

    def scale(self, scale: Scale):
        new_cell_locations = Location_list()
        for cell_location in self.cell_locations:
            new_cell_locations.append(self.space.scale(location=cell_location, scale=scale))
        self.cell_locations = new_cell_locations


class World_statistics(JsonObject):
    def __init__(self,
                 spatial_entropy: float = float(0),
                 spatial_espinometry: float = float(0),
                 spatial_connections: JsonList = None,
                 spatial_connections_derivative: JsonList = None,
                 spatial_centrality: JsonList = None,
                 spatial_centrality_derivative: JsonList = None,
                 visual_entropy: float = float(0),
                 visual_espinometry: float = float(0),
                 visual_connections: JsonList = None,
                 visual_connections_derivative: JsonList = None,
                 visual_centrality: JsonList = None,
                 visual_centrality_derivative: JsonList = None,
                 ITOR_potential: JsonList = None,
                 ITOR_direction: JsonList = None):
        self.spatial_entropy = spatial_entropy
        self.spatial_espinometry = spatial_espinometry
        if spatial_connections:
            self.spatial_connections = spatial_connections
        else:
            self.spatial_connections = JsonList(list_type=int)
        if spatial_connections_derivative:
            self.spatial_connections_derivative = spatial_connections_derivative
        else:
            self.spatial_connections_derivative = JsonList(list_type=int)
        if spatial_centrality:
            self.spatial_centrality = spatial_centrality
        else:
            self.spatial_centrality = JsonList(list_type=float)
        if spatial_centrality_derivative:
            self.spatial_centrality_derivative = spatial_centrality_derivative
        else:
            self.spatial_centrality_derivative = JsonList(list_type=float)

        self.visual_entropy = visual_entropy
        self.visual_espinometry = visual_espinometry
        if visual_connections:
            self.visual_connections = visual_connections
        else:
            self.visual_connections = JsonList(list_type=int)
        if visual_connections_derivative:
            self.visual_connections_derivative = visual_connections_derivative
        else:
            self.visual_connections_derivative = JsonList(list_type=int)
        if visual_centrality:
            self.visual_centrality = visual_centrality
        else:
            self.visual_centrality = JsonList(list_type=float)
        if visual_centrality_derivative:
            self.visual_centrality_derivative = visual_centrality_derivative
        else:
            self.visual_centrality_derivative = JsonList(list_type=float)
        if ITOR_potential:
            self.ITOR_potential = ITOR_potential
        else:
            self.ITOR_potential = JsonList(list_type=float)
        if ITOR_direction:
            self.ITOR_direction = ITOR_direction
        else:
            self.ITOR_direction = JsonList(list_type=Coordinates)

    @staticmethod
    def get_from_parameters_names(
            world_configuration_name: str,
            occlusions_name: str):
        if not type(world_configuration_name) is str:
            raise "incorrect type for world_configuration_name"
        if not type(occlusions_name) is str:
            raise "incorrect type for occlusions_name"
        return World_statistics.parse(json_dictionary=get_resource("world_statistics", world_configuration_name, occlusions_name))


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
            occlusions_name: str = None):
        wc = World_configuration.get_from_name(world_configuration_name)
        o = None
        wi = World_implementation.get_from_name(world_configuration_name, world_implementation_name)
        if occlusions_name:
            o = Cell_group_builder.get_from_name(world_configuration_name, occlusions_name, "occlusions")
        return World.get_from_parameters(wc, wi, o)

    @staticmethod
    def get_from_world_info(world_info: World_info):
        return World.get_from_parameters_names(world_configuration_name=world_info.world_configuration, world_implementation_name=world_info.world_implementation, occlusions_name=world_info.occlusions)


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

    def create_cell_group(self, cell_group_builder: Cell_group_builder):
        return Cell_group(world=self, cell_group_builder=cell_group_builder)

    def set_occlusions(self, occlusions: Cell_group_builder):
        for cell in self.cells:
            cell.occluded = cell.id in occlusions

    def __str__(self):
        return str(self.cells)

    def get_centrality(self, depth: int = 1) -> JsonList:
        cell_map = Cell_map(self.configuration.cell_coordinates)
        counters = [1 for x in self.cells]
        new_counters = [0 for x in self.cells]
        for i in range(depth):
            for cell_index, cell in enumerate(self.cells):
                if cell.occluded:
                    continue
                for connection in self.configuration.connection_pattern:
                    connection_index = cell_map[cell.coordinates + connection]
                    if connection_index == -1 or self.cells[connection_index].occluded:
                        continue
                    new_counters[cell_index] += counters[connection_index]
            counters = [x for x in new_counters]
        return JsonList(iterable=[c/max(counters) for c in counters], list_type=float)

    def get_centrality_derivative_product(self, depth: int = 1) -> JsonList:
        centrality = self.get_centrality(depth=depth)
        cell_map = Cell_map(self.configuration.cell_coordinates)
        centrality_derivative_product = [0.0 for x in self.cells]
        connection_pairs = self.get_connection_pattern_pairs()
        for cell_index, cell in enumerate(self.cells):
            if cell.occluded:
                continue
            centrality_derivative_product[cell_index] = 1
            for connection, opposite in connection_pairs:
                connection_index = cell_map[cell.coordinates + connection]
                if connection_index == -1 or self.cells[connection_index].occluded:
                    connection_value = 0
                else:
                    connection_value = centrality[connection_index]
                opposite_index = cell_map[cell.coordinates + opposite]
                if opposite_index == -1 or self.cells[opposite_index].occluded:
                    opposite_value = 0
                else:
                    opposite_value = centrality[opposite_index]
                centrality_derivative_product[cell_index] *= abs(connection_value-opposite_value)
        return [d/2 for d in centrality_derivative_product]

    def get_connection_pattern_pairs(self) -> list:
        pairs = []
        for connection in self.configuration.connection_pattern:
            if (-connection, connection) in pairs:
                continue
            pairs.append((connection, -connection))
        return pairs

    def entropy(self, occlusion_count: int, base: float = math.e):
        ent = 0
        cell_count = len(self.cells)
        probabilities = [occlusion_count/cell_count, (cell_count-occlusion_count)/cell_count]
        for p in probabilities:
            if p > 0:
                ent -= p * math.log(p, base)
        return ent / math.log(len(probabilities), base)

    def get_occlusion_count(self, target_entropy: float):
        cell_count = len(self.cells)
        for c in range(cell_count):
            if self.entropy(c) >= target_entropy:
                break
        return c

    def create_occlusions(self, entropy: float = -1, occlusion_count: int = -1, max_walk_length: int = 1, tries: int = 100, verbose: bool = False, seed: int = 0, occlude_disconnected: bool = True):

        if seed:
            random.seed(seed)

        if max_walk_length == 0:
            raise "walk length cannot be zero"

        if entropy == -1 and occlusion_count == -1:
            raise "entropy or occlusion count must be specified"

        if occlusion_count == -1:
            occlusion_count = self.get_occlusion_count(entropy)

        if verbose:
            print("Creating world with %i occlusions" % occlusion_count)

        class Visited:
            def __init__(self):
                self.cells = []

        def is_connected(cells, connection_pattern, cellmap: Cell_map, source: int, destination: int, visited):
            if cells[destination].occluded:
                return False
            if cells[source].occluded:
                return False
            curr = cells[source].coordinates
            for c in connection_pattern:
                i = cellmap[curr + c]
                if i >= 0 and not cells[i].occluded and i not in visited.cells:
                    visited.cells.append(i)
                    is_connected(cells, connection_pattern, cellmap, i, destination, visited)
            return True

        for t in range(tries):
            if verbose:
                print("Try %i: " % t, end="")
            walk = 0
            occlusions = Cell_group_builder()
            m = Cell_map(self.configuration.cell_coordinates)
            while len(occlusions) < occlusion_count:
                if walk == 0:
                    walk = random.randint(1, max_walk_length)
                    curr_cell = self.cells.random_cell()
                else:
                    alternatives = []
                    for a in self.configuration.connection_pattern:
                        i = m[curr_cell.coordinates + a]
                        if i >= 0:
                            if i not in occlusions:
                                alternatives.append(i)
                    if len(alternatives) == 0:
                        curr_cell = self.cells.random_cell()
                        walk = 0
                    else:
                        o = random.choice(alternatives)
                        occlusions.append(o)
                        curr_cell = self.cells[o]
                        walk -= 1
            self.set_occlusions(occlusions)
            visited = Visited()
            l = is_connected(self.cells, self.configuration.connection_pattern, m, 0, len(self.cells)-1, visited)
            if verbose:
                print("connection finished (%i connected cells) " % len(visited.cells), end="")
            if l and len(visited.cells) == len(self.cells.free_cells()):
                if verbose:
                    print("Success")
                return occlusions
            else:
                if verbose:
                    print("Fail")
        self.set_occlusions(Cell_group_builder())
        return None



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


