from json_cpp import JsonObject, JsonList
from .util import *


class Coordinates(JsonObject):
    def __init__(self,
                 x: int = 0,
                 y: int = 0):
        self.x = int(x)
        self.y = int(y)

    def __add__(self, o):
        c = Coordinates()
        c.x = self.x + o.x
        c.y = self.y + o.y
        return c

    def __sub__(self, o):
        c = Coordinates()
        c.x = self.x - o.x
        c.y = self.y - o.y
        return c

    def __neg__(self):
        c = Coordinates()
        c.x = -self.x
        c.y = -self.y
        return c

    def manhattan(self, o) -> int:
        return abs(o.x - self.x) + abs(o.y - self.y)


class Coordinates_list (JsonList):
    def __init__(self, iterable=None):
        JsonList.__init__(self, iterable, list_type=Coordinates)

    def get_x(self):
        x = []
        for coordinates in self:
            x.append(coordinates.x)
        return x

    def get_y(self):
        y = []
        for coordinates in self:
            y.append(coordinates.y)
        return y

