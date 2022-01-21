from json_cpp import JsonObject, JsonList
from .util import *
from .location import Location
from .coordinates import Coordinates, Coordinates_list

class Cell(JsonObject):
    def __init__(self,
                 cell_id: int = 0,
                 coordinates: Coordinates = None,
                 location: Location = None,
                 occluded: bool = False):
        if coordinates is None:
            coordinates = Coordinates()
        if location is None:
            location = Location()
        check_type(coordinates, Coordinates, "wrong type for coordinates")
        check_type(location, Location, "wrong type for location")
        check_type(cell_id, int, "wrong type for cell_id")
        check_type(occluded, bool, "wrong type for occluded")
        self.id = int(cell_id)
        self.coordinates = coordinates
        self.location = location
        self.occluded = occluded


class Cell_group_builder(JsonList):
    def __init__(self, iterable=None):
        JsonList.__init__(self, iterable, list_type=int)

    @staticmethod
    def get_from_name(world_name: str, name: str, *argv):
        if not type(world_name) is str:
            raise "incorrect type for world_name"
        if not type(name) is str:
            raise "incorrect type for name"
        return Cell_group_builder.parse(json_list=get_resource("cell_group", world_name, name, *argv))


class Cell_group(JsonList):
    def __init__(self, iterable = None,
                 world = None,
                 cell_group_builder: Cell_group_builder = None):
        if cell_group_builder is None:
            cell_group_builder = []
        JsonList.__init__(self, iterable, list_type=Cell)
        if world:
            for cell_id in cell_group_builder:
                self.append(world.cells[cell_id])

    def find(self, location: Location):
        closest = -1
        closest_distance = 0
        for index in range(len(self)):
            dist = location.dist(self[index].location)
            if dist < closest_distance or closest == -1:
                closest = index
                closest_distance = dist
        return closest

    def free_cells(self):
        return self.where("occluded", False)

    def occluded_cells(self):
        return self.where("occluded", True)

    def builder(self):
        cgb = Cell_group_builder()
        for cell in self:
            cgb.append(cell.id)
        return cgb


class Cell_map:
    def __init__(self, coordinates_list: Coordinates_list):
        self.coordinates = coordinates_list
        x = self.coordinates.get_x()
        y = self.coordinates.get_x()
        self.base_x = min(x)
        self.base_y = min(y)
        size_x = max(x) - self.base_x + 1
        size_y = max(y) - self.base_y + 1
        self.index = [[-1 for y in range(size_y)] for x in range(size_x)]
        for i, c in enumerate(coordinates_list):
            self.index[c.x][c.y] = i

    def __getitem__(self, coordinates: Coordinates) -> int:
        try:
            return self.index[coordinates.x][coordinates.y]
        except:
            return -1

