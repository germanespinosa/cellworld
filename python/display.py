import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
from map import Map

class Display:
    def __init__ (self, heat_map, cmap = "Purples"):
        self.type = heat_map.world.cells[0]["cell_type"]
        self.heat_map = heat_map
        plt.style.use("dark_background")
        if self.type == 0:
            self.fig, self.ax = plt.subplots(1, 1, figsize=(10, 9))
        else:
            self.fig, self.ax = plt.subplots(1, 1, figsize=(10, 10))
        self.map = Map(heat_map.world)
        self.ax.axes.xaxis.set_visible(False)
        self.ax.axes.yaxis.set_visible(False)
        self.values = []
        self.colors = []
        for cell in heat_map.world.cells:
            self.values.append(heat_map.values[heat_map.get_index(cell["coordinates"])])
        unique_values = list(set(self.values))
        unique_values.sort()
        self.color_index = {v: i for i, v in enumerate(unique_values)}
        self.color_map = cm.get_cmap('Purples')
        self.colors = self.color_map(np.linspace(0, 1, len(unique_values)))
        self.min = min(unique_values)
        self.max = max(unique_values)
        self.x = []
        self.y = []
        self.c = []
        self.o = []
        for i, cell in enumerate(heat_map.world.cells):
            self.x.append(cell["location"]["x"])
            self.y.append(cell["location"]["y"])
            if cell["occluded"] == 0:
                value = heat_map.values[heat_map.get_index(cell["coordinates"])]
                index = self.color_index[value]
                color = self.colors[index]
                self.c.append(color)
            else:
                self.c.append("black")
            self.o.append("lightgrey")

    def show(self):
        if self.type == 0:
            plt.scatter(self.x, self.y, c=self.c, alpha=1, marker="h", s=850, edgecolors=self.o)
        else:
            plt.scatter(self.x, self.y, c=self.c, alpha=1, marker="s", s=1300, edgecolors=self.o)

        plt.show()

    def set_cell_color(self, id_or_coordinates, color):
        if type(id_or_coordinates) is int:
            self.c[id_or_coordinates] = color
        else:
            self.c[self.map.find(id_or_coordinates)] = color

    def set_cell_outline(self, id_or_coordinates, color):
        if type(id_or_coordinates) is int:
            self.o[id_or_coordinates] = color
        else:
            self.o[self.map.find(id_or_coordinates)] = color

    def close(self):
        plt.close()
