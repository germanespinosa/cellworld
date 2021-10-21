from .util import *
from .location import Location
from .coordinates import Coordinates, Coordinates_list


class Cell:
    def __init__(self, cell_id, coordinates=Coordinates(), location=Location(), occluded=False):
        self.id = int(cell_id)
        self.coordinates = coordinates
        self.location = location
        self.occluded = occluded

    def get(cell):
        return Cell(cell_id=cell["id"], coordinates=Coordinates.get(cell["coordinates"]), location=Location.get(cell["location"]), occluded=cell["occluded"])

    def __str__(self):
        return '{"id":%d,"coordinates":%s,"location":%s,"occluded":%s}' % (self.id, self.coordinates, self.location, "true" if self.occluded else "false")


class Cell_group_builder:
    def __init__(self):
        self.cell_ids = []

    def get(cell_ids):
        if type(cell_ids) is not list:
            raise "incorrect type for cell_ids"
        cg = Cell_group_builder()
        for cell_id in cell_ids:
            if type(cell_id) is not int:
                raise "incorrect type for cell_ids"
            cg.cell_ids.append(cell_id)
        return cg

    def get_from_name(world_name, name, *argv):
        if not type(world_name) is str:
            raise "incorrect type for world_name"
        if not type(name) is str:
            raise "incorrect type for name"
        return Cell_group_builder.get(get_resource("cell_group", world_name, name, *argv))

    def __str__(self):
        return "[" + ",".join([str(x) for x in self.cell_ids]) + "]"


class Cell_group:
    def __init__(self, world, cell_group_builder=[]):
        self.world = world
        self.cell_ids = cell_group_builder.cell_ids

    def add(self, cell):
        if cell.id in self.cell_ids:
            return False
        self.cell_ids.append(cell.id)
        return True

    def __str__(self):
        cgb = Cell_group_builder()
        cgb.cell_ids = self.cell_ids
        return str(cgb.cell_ids)


class Cell_map:

    def __init__(self, coordinates_list):
        check_type(coordinates_list, Coordinates_list, "incorrect type for coordinates_list")
        self.coordinates = coordinates_list
        x = self.coordinates.get_x()
        y = self.coordinates.get_x()
        self.base_x = min(x)
        self.base_y = min(y)
        size_x = max(x) - self.base_x + 1
        size_y = max(y) - self.base_y + 1
        self.index = [[-1 for y in range(size_y)] for x in range(size_x)]
        for i, c in enumerate(coordinates_list.coordinates):
            self.index[c.x][c.y] = i

    def __getitem__(self, coordinates):
        check_type(coordinates, Coordinates, "incorrect type for coordinates")
        return self.index[coordinates.x][coordinates.y]

