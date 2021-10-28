from .location import Location, Locations_list
from .util import *
import math


class Shape:
    def __init__(self, sides=6):
        self.sides = int(sides)

    @staticmethod
    def get(shape):
        return Shape(sides=shape["sides"])

    def __str__(self):
        return '{"sides":%d}' % self.sides


class Transformation:
    def __init__(self, size=0, rotation=0):
        self.size = float(size)
        self.rotation = float(rotation)

    @staticmethod
    def get(transformation):
        t = Transformation()
        t.size = transformation["size"]
        t.rotation = transformation["rotation"]
        return t

    @staticmethod
    def get_transformations(n, rotation=0, size=1):
        return [Transformation(size, rotation + a * (360 / n)) for a in range(n)]

    def __str__(self):
        return '{"size":%f,"rotation":%f}' % (self.size, self.rotation)


class Space:
    def __init__(self, center=Location(), shape=Shape(), transformation=Transformation()):
        check_type(center, Location, "incorrect type for center")
        check_type(shape, Shape, "incorrect type for shape")
        check_type(transformation, Transformation, "incorrect type for transformation")
        self.center = center
        self.shape = shape
        self.transformation = transformation

    @staticmethod
    def get(space):
        s = Space()
        s.center = space["center"]
        s.shape = space["shape"]
        s.transformation = space["transformation"]
        return s

    @staticmethod
    def transform_to(location, src_space, dst_space):
        check_type(location, Location, "incorrect type for location")
        check_type(src_space, Space, "incorrect type for src_space")
        check_type(dst_space, Space, "incorrect type for dst_space")
        size_ratio = dst_space.transformation.size / src_space.transformation.size
        rotation = math.radians(dst_space.transformation.rotation - src_space.transformation.rotation)
        dist = src_space.center.dist(location)
        theta = src_space.center.atan(location)
        new_location = Location(dst_space.center.x, dst_space.center.y)
        return new_location.move(theta + rotation, dist * size_ratio)

    def __str__(self):
        return '{"center":%s,"shape":%s,"transformation":%s}' % (self.center, self.shape, self.transformation)


class Polygon:

    def __init__(self, center, sides=0, radius=1.0, rotation=0, vertices=Locations_list()):
        check_type(center, Location, "incorrect type for center")
        check_type(sides, int, "incorrect type for sides")
        check_type(vertices, Locations_list, "incorrect type for sides")
        self.center = center
        self.vertices = vertices
        self.radius = float(radius)
        if sides > 0:
            if len(self.vertices.locations) != 0:
                raise "cannot use sides and vertices together"
            rotation = float(rotation)
            if sides == 0:
                raise "incorrect parameters"
            theta = math.radians(rotation)
            inc = 2.0 * math.pi / sides
            for s in range(sides):
                c = center
                self.vertices.locations.append(c.move(theta, radius))
                theta += inc
        else:
            if len(self.vertices.locations) == 0:
                raise "must specify either sides or vertices"

    def move(self, location=None, theta=None, dist=None):
        check_type(location, Location, "incorrect type for location")
        dif = Location(0, 0)
        if location is None:
            if theta is None or dist is None:
                raise "incorrect parameters"
            dif.move(theta, dist)
        else:
            dif = location - self.center
        self.center = location
        for v in self.vertices:
            v = v + dif

    @staticmethod
    def get(polygon):
        return Polygon(center=Location.get(polygon["center"]), vertices=Locations_list.get(polygon["vertices"]))


class Polygon_list:
    def __init__(self):
        self.polygons = []

    def __getitem__(self, index):
        return self.polygons[index]

    @staticmethod
    def get(polygon_list):
        pl = Polygon_list()
        for polygon in polygon_list:
            pl.polygons.append(Polygon.get(polygon))
        return pl

    def __str__(self):
        return "[" + ",".join([str(x) for x in self.polygons]) + "]"