from .location import Location, Location_list
from .shape import Polygon, Polygon_list
from .world import World
from .util import *
from .cell import Cell_group
from json_cpp import JsonList


class Location_visibility:

    def __init__(self, occlusions: Polygon_list):
        self.occlusions = occlusions

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
        free_cells = cells.free_cells() #.get("location")
        visible_cells = JsonList()

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
        hidden_cells = JsonList()
        free_locations_set = set(cells.free_cells().get("id"))
        visible_free_locations_set = set(self.visible_cells(src, cells).get('id'))
        hidden_id_list = list(free_locations_set.difference(visible_free_locations_set))
        for id in hidden_id_list:
            hidden_cells.append(cells[id])

        return hidden_cells



