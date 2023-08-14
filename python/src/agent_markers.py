from matplotlib.path import Path
from .location import Location, Location_list
from .shape import to_radians


class Agent_markers:

    @staticmethod
    def icon_folder() -> str:
        import pkg_resources
        path = 'files'  # always use slash
        filepath = pkg_resources.resource_filename("cellworld", path)
        return filepath

    @staticmethod
    def icon_box(location:Location, base_size: float, rotation: float):
        corners_angles = [to_radians(45+rotation), to_radians(135+rotation), to_radians(225+rotation), to_radians(315+rotation)]
        corners = Location_list([Location(location.x, location.y).move(ca, base_size) for ca in corners_angles])
        x = corners.get_x()
        y = corners.get_y()
        return [min(x), max(x), min(y), max(y)]

    @staticmethod
    def robot(marker_size: float = 1.0) -> Path:
        verts = [
            (-0.25 * marker_size, -1.0 * marker_size),
            (-0.25 * marker_size, 1.0 * marker_size),
            (0.25 * marker_size, 1.0 * marker_size),
            (0.25 * marker_size, - 1.0 * marker_size),
            (-0.25 * marker_size, - 1.0 * marker_size),

            (-0.25 * marker_size, - .1 * marker_size),
            (-0.45 * marker_size, - .1 * marker_size),
            (-0.45 * marker_size, - .5 * marker_size),
            (-0.55 * marker_size, - .5 * marker_size),
            (-0.55 * marker_size, .5 * marker_size),
            (-0.45 * marker_size, .5 * marker_size),
            (-0.45 * marker_size, .1 * marker_size),
            (-0.25 * marker_size, .1 * marker_size),
            (-0.25 * marker_size, -.1 * marker_size),

            (0.25 * marker_size, -.1 * marker_size),
            (0.45 * marker_size, -.1 * marker_size),
            (0.45 * marker_size, -.5 * marker_size),
            (0.55 * marker_size, -.5 * marker_size),
            (0.55 * marker_size, .5 * marker_size),
            (0.45 * marker_size, .5 * marker_size),
            (0.45 * marker_size, .1 * marker_size),
            (0.25 * marker_size, .1 * marker_size),
            (0.25 * marker_size, -.1 * marker_size),

            (0.5 * marker_size, 1.5 * marker_size),
            (0 * marker_size, 1.0 * marker_size),
            (-0.5 * marker_size, 1.5 * marker_size),
        ]

        codes = [
            Path.MOVETO,
            Path.LINETO,
            Path.LINETO,
            Path.LINETO,
            Path.CLOSEPOLY,
            Path.MOVETO,
            Path.LINETO,
            Path.LINETO,
            Path.LINETO,
            Path.LINETO,
            Path.LINETO,
            Path.LINETO,
            Path.LINETO,
            Path.CLOSEPOLY,
            Path.MOVETO,
            Path.LINETO,
            Path.LINETO,
            Path.LINETO,
            Path.LINETO,
            Path.LINETO,
            Path.LINETO,
            Path.LINETO,
            Path.CLOSEPOLY,
            Path.MOVETO,
            Path.LINETO,
            Path.LINETO,
        ]

        return Path(verts, codes)

    @staticmethod
    def arrow(marker_size: float = 1.0) -> Path:
        verts = [(.0 * marker_size, .5 * marker_size),
                 (.5 * marker_size, -.5 * marker_size),
                 (.0 * marker_size, -.25 * marker_size),
                 (-.5 * marker_size, -.5 * marker_size),
                 (.0 * marker_size, .5 * marker_size)]
        codes = [
            Path.MOVETO,  # begin drawing
            Path.LINETO,  # straight line
            Path.LINETO,  # straight line
            Path.LINETO,  # straight line
            Path.CLOSEPOLY
        ]
        return Path(verts, codes)

    @staticmethod
    def mouse(marker_size: float = 1.0) -> Path:
        verts = [(-.8 * marker_size, .0 * marker_size),
                 (-.3 * marker_size, -1.0 * marker_size),
                 (.0 * marker_size, -1.5 * marker_size),
                 (.3 * marker_size, -1.0 * marker_size),
                 (.3 * marker_size, -1.0 * marker_size),
                 (.8 * marker_size, .0 * marker_size),
                 (2.0 * marker_size, -.1 * marker_size),
                 (.8 * marker_size, 2.0 * marker_size),
                 (.6 * marker_size, .0 * marker_size),
                 (.3 * marker_size, .5 * marker_size),
                 (-0.3 * marker_size, .5 * marker_size),
                 (-.6 * marker_size, .0 * marker_size),
                 (-.8 * marker_size, 2 * marker_size),
                 (-2 * marker_size, -.1 * marker_size),
                 (-.8 * marker_size, .0 * marker_size)]
        codes = [
            Path.MOVETO,  # begin drawing
            Path.LINETO,  # straight line
            Path.CURVE3,  # curve
            Path.CURVE3,  # curve
            Path.LINETO,  # straight line
            Path.LINETO,  # straight line

            Path.CURVE4,  # curve
            Path.CURVE4,  # curve
            Path.CURVE4,  # curve

            Path.CURVE4,  # curve
            Path.CURVE4,  # curve
            Path.CURVE4,  # curve

            Path.CURVE4,  # curve
            Path.CURVE4,  # curve
            Path.CURVE4,  # curve
        ]
        return Path(verts, codes)


