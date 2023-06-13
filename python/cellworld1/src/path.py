from json_cpp import JsonObject, JsonList
from .coordinates import Coordinates, Coordinates_list
from .world import World


class Paths(JsonObject):
    def __init__(self):
        self.moves = Coordinates_list()
        self.steps = JsonList(list_type=int)
        JsonObject.__init__(self)

