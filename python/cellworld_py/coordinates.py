class Coordinates:
    def __init__(self, x=0, y=0):
        self.x = int(x)
        self.y = int(y)

    def get(coordinates):
        c = Coordinates()
        c.x = coordinates["x"]
        c.y = coordinates["y"]
        return c

    def __str__(self):
        return '{"x":%d,"y":%d}' % (self.x, self.y)

    def __add__(self, o):
        c = Coordinates()
        c.x = self.x + o.x
        c.y = self.y + o.y
        return c


class Coordinates_list:
    def __init__(self):
        self.coordinates = []

    def __getitem__(self, index):
        return self.coordinates[index]

    def get(coordinates_list):
        cl = Coordinates_list()
        for coordinates in coordinates_list:
            cl.coordinates.append(Coordinates.get(coordinates))
        return cl

    def get_x(self):
        x = []
        for coordinates in self.coordinates:
            x.append(coordinates.x)
        return x

    def get_y(self):
        y = []
        for coordinates in self.coordinates:
            y.append(coordinates.y)
        return y

    def __str__(self):
        return "[" + ",".join([str(x) for x in self.coordinates]) + "]"

