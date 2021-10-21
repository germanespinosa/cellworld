import math
from matplotlib.patches import RegularPolygon
import matplotlib.pyplot as plt
from .world import World_configuration, World_implementation, World

class Display:
    def __init__(self, world, fig_size=(10, 10), padding=.1):
        if not isinstance(world, World):
            raise "incorrect type for world"
        self.world = world
        self.fig = plt.figure(figsize=fig_size)
        self.ax = self.fig.add_subplot(111)

        #self.ax.axes.xaxis.set_visible(False)
        #self.ax.axes.yaxis.set_visible(False)

        xcenter = world.implementation.center.x
        ycenter = world.implementation.center.y

        hsize = world.implementation.space.transformation.size / 2
        pad = hsize * padding

        xmin = xcenter - hsize - pad
        xmax = xcenter + hsize + pad

        ymin = ycenter - hsize - pad
        ymax = ycenter + hsize + pad

        self.ax.set_xlim(xmin=xmin, xmax=xmax)
        self.ax.set_ylim(ymin=ymin, ymax=ymax)

        ssides = world.implementation.space.shape.sides
        srotation = math.radians(0-world.implementation.space.transformation.rotation)

        csides = world.cell_shape.sides
        crotation = math.radians(0 - world.implementation.cell_transformation.rotation - srotation)


        csize = world.implementation.cell_transformation.size / 2
        ssize = hsize

        for cell in self.world.cells:
            color = "black" if cell.occluded else "white"
            self.ax.add_patch(RegularPolygon((cell.location.x, cell.location.y), csides, csize, facecolor=color, edgecolor="grey", orientation=crotation, zorder=-2, linewidth=1))
        self.ax.add_patch(RegularPolygon((xcenter, ycenter), ssides, ssize, facecolor=(1, 0, 0, 0), edgecolor="black", orientation=srotation, zorder=-1))
        plt.tight_layout()

