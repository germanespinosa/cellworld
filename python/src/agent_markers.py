from matplotlib.path import Path


class Agent_markers:

    @staticmethod
    def robot(marker_size=1):
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
    def mouse(marker_size=1.0):
        verts = [(-.8, .0), (-.3, -1.0), (.0, -1.5), (.3, -1.0), (.3, -1.0), (.8, .0),
                 (2, -.1), (.8, 2), (.6, .0),
                 (.3, .5), (-0.3, .5), (-.6, .0),
                 (-.8, 2), (-2, -.1), (-.8, .0)]
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
