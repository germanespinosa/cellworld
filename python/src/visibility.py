from .location import Location, Location_list
from .shape import Polygon, Polygon_list
from .world import World
from .util import *


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

    def visible_locations(self, loc, world):
        """
        Finds all visible cells from specified location arg (loc)
        :return
        A list of all the visible cell ids
        """
        free_locations = world.cells.free_cells().get("location")
        location_list = world.cells.get("location")
        visible_id_list = []
        for free_location in free_locations:
            if self.is_visible(loc, free_location):
                visible_id_list.append(location_list.index(free_location))
        return visible_id_list

    def invisible_locations(self, loc, world):
        """
        Finds all hidden cells from specified location arg (loc)
        :return:
        A list of all the hidden cell ids
        """
        all_locations_set = set(world.cells.free_cells().get("id"))
        free_locations_set = set(self.visible_locations(loc, world))
        invisible_id_list = list(all_locations_set.difference(free_locations_set))
        return invisible_id_list


