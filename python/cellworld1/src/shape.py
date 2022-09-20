from json_cpp import JsonObject, JsonList
from .location import Location, Location_list, segments_intersect, Scale
from .util import *
import math


class Shape(JsonObject):
    def __init__(self, sides=6):
        self.sides = int(sides)


class Transformation(JsonObject):
    def __init__(self, size=0.0, rotation=0.0):
        self.size = float(size)
        self.rotation = float(rotation)


class Transformation_list(JsonList):
    def __init__(self, iterable=None, n=None, size=0.0, rotation=0.0):
        JsonList.__init__(self, iterable, list_type=Transformation)
        if n:
            for a in range(n):
                self.append(Transformation(float(size), float(rotation) + float(a) * (360.0 / float(n))))


class Space(JsonObject):
    def __init__(self,
                 center: Location = None,
                 shape: Shape = None,
                 transformation: Transformation = None):
        self.center = center if center else Location()
        self.shape = shape if shape else Shape()
        self.transformation = transformation if transformation else Transformation()

    @staticmethod
    def transform_to(location: Location, src_space, dst_space):
        check_type(src_space, Space, "incorrect type for src_space")
        check_type(dst_space, Space, "incorrect type for dst_space")
        size_ratio = dst_space.transformation.size / src_space.transformation.size
        rotation = math.radians(dst_space.transformation.rotation - src_space.transformation.rotation)
        dist = src_space.center.dist(location)
        theta = src_space.center.atan(location)
        new_location = Location(dst_space.center.x, dst_space.center.y)
        return new_location.move(theta + rotation, dist * size_ratio)

    def scale(self, location:Location, scale: Scale):
        diff = (location - self.center)
        scaled = diff * scale
        new_location = scaled + self.center
        return new_location

class Polygon:

    def __init__(self,
                 center: Location,
                 sides: int = 0,
                 radius: float = 1.0,
                 rotation: float = 0.0,
                 vertices: Location_list = None):
        if vertices is None:
            vertices = Location_list()
        self.center = center
        self.vertices = vertices
        self.radius = float(radius)
        if sides > 0:
            if len(self.vertices) != 0:
                raise "cannot use sides and vertices together"
            rotation = float(rotation)
            if sides == 0:
                raise "incorrect parameters"
            theta = math.radians(rotation)
            inc = 2.0 * math.pi / sides
            for s in range(sides):
                c = center.copy()
                self.vertices.append(c.move(theta, radius))
                theta += inc
        else:
            if len(self.vertices) == 0:
                raise "must specify either sides or vertices"

    def move(self,
             location: Location = None,
             theta: float = None,
             dist: float = None):
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

    def contains(self, location: Location):
        dist = self.center.dist(location)
        if dist > self.radius:
            return False
        inner_radius = self.center.dist(segment=(self.vertices[0], self.vertices[1]))
        if dist < inner_radius:
            return True
        for i in range(1, len(self.vertices)):
            if segments_intersect((self.center, location), (self.vertices[i], self.vertices[i-1])):
                return False
        if segments_intersect((self.center, location), (self.vertices[-1], self.vertices[0])):
            return False
        return True

    def is_between(self,
                   src: Location = None,
                   dst: Location = None,
                   theta: float = None,
                   dist: float = None):
        if dst is not None:
            theta = src.atan(dst)
            dist = src.dist(dst)
        else:
            if theta is None or dist is None:
                raise "either dst or theta and dist should be used"
        dist_center = src.dist(self.center)
        theta_center = src.atan(self.center)
        diff_theta_center, direction_center = angle_difference(theta, theta_center)
        if dist < dist_center - self.radius:
            return False
        for v in self.vertices:
            vertex_distance = src.dist(v)
            if vertex_distance < dist:
                theta_vertex = src.atan(v)
                diff_theta_vertex, direction_vertex = angle_difference(theta, theta_vertex);
                if direction_center == -direction_vertex:
                    if diff_theta_center + diff_theta_vertex < math.pi:
                        return True
        return False


class Polygon_list(JsonList):

    def __init__(self, iterable=None):
        JsonList.__init__(self, iterable, list_type=Polygon)

    @staticmethod
    def get_polygons(centers: Location_list, sides: int, radius: float, rotation: float):
        polygon_list = Polygon_list()
        for center in centers:
            polygon_list.append(Polygon(center,sides=sides, rotation=rotation, radius=radius))
        return polygon_list
