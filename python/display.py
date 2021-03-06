import warnings
import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
from map import Map
from IPython.display import clear_output
from time import sleep
warnings.filterwarnings("ignore", category=np.VisibleDeprecationWarning)

class Display:
    def __init__ (self, heat_map, cmap = "Purples"):
        self.type = heat_map.world.cells[0]["cell_type"]
        self.heat_map = heat_map
        self.map = Map(heat_map.world)
        plt.style.use("dark_background")
        self.values = []
        self.colors = []
        self.size = (4500, 2400)
        self.extras = []
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

    def prepare(self):
        self.ax.axes.xaxis.set_visible(False)
        self.ax.axes.yaxis.set_visible(False)
        if self.type == 0:
            plt.scatter(self.x, self.y, c=self.c, alpha=1, marker="h", s=850, edgecolors=self.o, linewidths=1)
        else:
            plt.scatter(self.x, self.y, c=self.c, alpha=1, marker="s", s=1170, edgecolors=self.o , linewidths=2)

        for x,y,c in self.extras:
            plt.plot(x, y, c=c)

    def show(self):
        if self.type == 0:
            self.fig, self.ax = plt.subplots(1, 1, figsize=(10, 9))
        else:
            self.fig, self.ax = plt.subplots(1, 1, figsize=(10, 10))
        self.prepare()
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


    def convert_location (self, location):
        return location["x"] / 980 * self.size[0] - self.size[0] / 2, (1 - location["y"] / 862) * self.size[1] - self.size[1] / 2

    def add_trajectory(self, trajectory, color="red"):
        if self.type == 0:
            x = [self.convert_location(step["location"])[0] for step in trajectory]
            y = [self.convert_location(step["location"])[1] for step in trajectory]
            self.extras.append((x,y,color))

    def show_trajectory(self, trajectory, color="red"):
        if self.type == 0:
            x = [self.convert_location(step["location"])[0] for step in trajectory]
            y = [self.convert_location(step["location"])[1] for step in trajectory]

            for i in range(len(x)):
                clear_output(wait=True)
                self.fig, self.ax = plt.subplots(1, 1, figsize=(10, 9))
                self.prepare()
                plt.plot(x[:i], y[:i], c=color)
                plt.show()
                sleep(.1)