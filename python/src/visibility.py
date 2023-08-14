from .world import World
from .location import Location
from .shape import Polygon_list, Polygon
from .cell import Cell_group
from .util import to_radians
from .util import angle_difference


class Location_visibility:

    def __init__(self, occlusions: Polygon_list):
        self.occlusions = occlusions

    @staticmethod
    def from_world(world: World):
        occlusions = world.cells.occluded_cells()
        occlusion_locations = occlusions.get("location")
        occlusion_polygons = Polygon_list.get_polygons(occlusion_locations,
                                                       sides=world.configuration.cell_shape.sides,
                                                       radius=world.implementation.cell_transformation.size / 2,
                                                       rotation=world.implementation.space.transformation.rotation + world.implementation.cell_transformation.rotation)
        return Location_visibility(occlusion_polygons)

    def is_visible(self, src: Location, dst: Location) -> bool:
        theta = src.atan(dst)
        dist = src.dist(dst)
        for occlusion in self.occlusions:
            if occlusion.is_between(src, theta=theta, dist=dist):
                return False
        return True

    def visible_cells(self, src: Location, cells: Cell_group) -> Cell_group:
        """
        Finds all visible cells from specified location
        :return
        A list of all the visible cell ids
        """
        free_cells = cells.free_cells()
        visible_cells = Cell_group()

        for free_cell in free_cells:
            if self.is_visible(src, free_cell.location):
                visible_cells.append(free_cell)

        return visible_cells

    def hidden_cells(self, src: Location, cells: Cell_group) -> Cell_group:
        """
        Finds all hidden cells from specified location arg (loc)
        :return:
        A list of all the hidden cell ids
        """
        hidden_cells = Cell_group()
        free_cells = cells.free_cells()
        visible_cells_ids = self.visible_cells(src, cells).get('id')
        for cell in free_cells:
            if cell.id not in visible_cells_ids:
                hidden_cells.append(cell)

        return hidden_cells



