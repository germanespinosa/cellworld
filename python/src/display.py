import numpy
from matplotlib.patches import RegularPolygon
import matplotlib.pyplot as plt
import matplotlib.colors
from matplotlib.transforms import Affine2D
from .world import *
from .experiment import *
from .agent_markers import *

class Display:

    def __init__(self,
                 world: World,
                 fig_size: tuple = (12, 10),
                 padding: float = .1,
                 show_axes: bool = False,
                 cell_color="white",
                 occlusion_color="black",
                 background_color="white",
                 habitat_color="white",
                 cell_edge_color="black",
                 habitat_edge_color="black",
                 animated: bool = False):
        if animated:
            plt.ion()
        self.agents = dict()
        self.agents_markers = dict()
        self.agents_markers["predator"] = Agent_markers.robot()
        self.agents_markers["prey"] = Agent_markers.mouse()
        self.animated = animated
        self.world = world
        self.fig = plt.figure(figsize=fig_size)
        self.ax = self.fig.add_subplot(111)
        self.ax.axes.xaxis.set_visible(show_axes)
        self.ax.axes.yaxis.set_visible(show_axes)
        self.agents_trajectories = Trajectories()
        xcenter = world.implementation.space.center.x
        ycenter = world.implementation.space.center.y

        hsize = world.implementation.space.transformation.size / 2
        pad = hsize * padding

        xmin = xcenter - hsize - pad
        xmax = xcenter + hsize + pad

        ymin = ycenter - hsize - pad
        ymax = ycenter + hsize + pad

        self.ax.set_xlim(xmin=xmin, xmax=xmax)
        self.ax.set_ylim(ymin=ymin, ymax=ymax)
        self.ax.set_facecolor(background_color)
        ssides = world.implementation.space.shape.sides
        srotation = math.radians(0-world.implementation.space.transformation.rotation)

        csides = world.configuration.cell_shape.sides
        crotation = math.radians(0 - world.implementation.cell_transformation.rotation - srotation)


        csize = world.implementation.cell_transformation.size / 2
        ssize = hsize

        for cell in self.world.cells:
            color = occlusion_color if cell.occluded else cell_color
            self.ax.add_patch(RegularPolygon((cell.location.x, cell.location.y), csides, csize, facecolor=color, edgecolor=cell_edge_color, orientation=crotation, zorder=-2, linewidth=1))
        self.ax.add_patch(RegularPolygon((xcenter, ycenter), ssides, ssize, facecolor=habitat_color, edgecolor=habitat_edge_color, orientation=srotation, zorder=-3))
        plt.tight_layout()

    def add_trajectories(self, trajectories: Trajectories, colors={}):
        agents = trajectories.get_agent_names()
        for index, agent in enumerate(agents):
            locations = trajectories.get_agent_trajectory(agent).get("location")
            x = locations.get("x")
            y = locations.get("y")
            color = list(matplotlib.colors.cnames.keys())[index]
            if agent in colors:
                color = colors[agent]
            for i in range(len(x)-1):
                lcolor = None
                if type(color) is numpy.ndarray:
                    lcolor = color[i]
                else:
                    lcolor = color
                self.ax.plot([x[i], x[i+1]], [y[i], y[i+1]], color=lcolor, alpha=.5, linewidth=3)

    def circle(self, location: Location, radius: float, color):
        circle_patch = plt.Circle((location.x, location.y), radius, color=color)
        return self.ax.add_patch(circle_patch)

    def arrow(self, beginning: Location, theta: float, dist: float, color, head_width: float = .02):
        ending = beginning.copy().move(theta=theta, dist=dist)
        length = ending - beginning
        return self.ax.arrow(beginning.x, beginning.y, length.x, length.y, color=color, head_width=head_width, length_includes_head=True)

    def agent(self, step: Step = None, agent_name: str = None, location: Location = None, rotation: float = None, color=None, size: float = 40.0, show_trajectory: bool = True):
        if step:
            agent_name = step.agent_name
            location = step.location
            rotation = step.rotation

        # if show_trajectory:
        #     self.agents_trajectories.append(step)
        #     x = self.agents_trajectories.get_agent_trajectory(agent_name).get("location").get("x")
        #     y = self.agents_trajectories.get_agent_trajectory(agent_name).get("location").get("y")
        #     self.agents[agent_name], = self.ax.plot(x, y, c=color)

        if agent_name not in self.agents:
            self.agents[agent_name], = self.ax.plot(location.x, location.y, marker=Agent_markers.robot(), c=color, markersize=size)

        t = Affine2D().rotate_deg_around(0, 0, -rotation)
        self.agents[agent_name].set_marker(Agent_markers.robot(1.0).transformed(t))
        self.agents[agent_name].set_xdata(location.x)
        self.agents[agent_name].set_ydata(location.y)

    def update(self):
        if self.animated:
            self.fig.canvas.draw()
            self.fig.canvas.flush_events()
            plt.pause(.001)
