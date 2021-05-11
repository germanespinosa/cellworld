import matplotlib.pyplot as plt
from matplotlib.patches import Rectangle
from world import World
from heat_map import Heat_map


class Display:
    def __init__(self, world, show_occlusions=True, values=[], cmap="Purples", figsize=(10, 10)):
        self.cmap = cmap
        self.world = world
        self.values = Heat_map(world)
        self.fig, self.ax = plt.subplots(1, 1, figsize=figsize)
        for i, value in enumerate(values):
            self.values[i] = value
        self.im = self.ax.imshow(self.values.values, cmap=self.cmap, interpolation='nearest')
        self.ax.axes.xaxis.set_visible(False)
        self.ax.axes.yaxis.set_visible(False)
        self.show_occlusions = show_occlusions


    def add_line(self, coord1, coord2, color="red", arrow=False):
        if not self.world.is_valid(coord1) or not self.world.is_valid(coord2):
            return
        x1, y1 = self.values.get_index(coord1)
        x2, y2 = self.values.get_index(coord2)
        dx = x2 - x1
        dy = y2 - y1
        self.ax.arrow(x1, y1, dx, dy,
                  head_width=0.2 if arrow else 0,
                  width=0.05,
                  color=color,
                  fill=True,
                  length_includes_head=True)

    def add_patch(self, coord, color):
        if not self.world.is_valid(coord):
            return
        x, y = self.values.get_index(coord)
        self.ax.add_patch(Rectangle((y - .5, x - .5), 1, 1, fill=True, color=color, lw=0))

    def add_outline(self, coord, color):
        if not self.world.is_valid(coord):
            return
        x, y = self.values.get_index(coord)
        self.ax.add_patch(Rectangle((y - .5, x - .5), 1, 1, fill=False, edgecolor=color, lw=2))


    def add_occlusions(self, color="black"):
        for cell in self.world.cells:
            if cell["occluded"]:
                self.add_patch(cell["coordinates"], color)

    def add_squares(self, color="lightgrey"):
        for cell in self.world.cells:
            self.add_outline(self.values.get_index(cell["coordinates"]), color)

    def save(self, file_name):
        self.fig.savefig(file_name)

    def show(self):
        if self.show_occlusions:
            self.add_occlusions()
        plt.show()

    def close(self):
        plt.close()