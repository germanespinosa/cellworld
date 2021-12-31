from matplotlib.path import Path


class Agent_markers:

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
