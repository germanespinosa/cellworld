import math

class Location:
    def __init__(self, x=0, y=0):
        self.x = float(x)
        self.y = float(y)

    @staticmethod
    def get(location):
        l = Location()
        l.x = location["x"]
        l.y = location["y"]
        return l

    def __str__(self):
        return '{"x":%f,"y":%f}' % (self.x, self.y)

    def __add__(self, o):
        c = Location()
        c.x = self.x + o.x
        c.y = self.y + o.y
        return c

    def __sub__(self, o):
        c = Location()
        c.x = self.x - o.x
        c.y = self.y - o.y
        return c

    def move(self, theta, dist):
        self.x += math.sin(theta) * dist
        self.y += math.cos(theta) * dist
        return self

    def atan(self, loc):
        v = loc-self
        return math.atan2(v.x, v.y)

    def dist(self, loc):
        v = loc-self
        return (v.x ** 2 + v.y ** 2) ** .5


class Locations_list:
    def __init__(self):
        self.locations = []

    def __getitem__(self, index):
        return self.locations[index]

    @staticmethod
    def get(location_list):
        ll = Locations_list()
        for location in location_list:
            ll.locations.append(Location.get(location))
        return ll

    def get_x(self):
        x = []
        for location in self.locations:
            x.append(location.x)
        return x

    def get_y(self):
        y = []
        for location in self.locations:
            y.append(location.y)
        return y

    def __str__(self):
        return "[" + ",".join([str(x) for x in self.locations]) + "]"
