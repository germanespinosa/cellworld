from .location import Location
from .util import *
import math

class Shape:
    def __init__(self, sides=6):
        self.sides = int(sides)

    @staticmethod
    def get(shape):
        return Shape(sides=shape["sides"])

    def __str__(self):
        return '{"sides":%d}' % (self.sides, self.rotation)


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


