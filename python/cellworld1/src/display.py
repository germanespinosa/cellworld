import numpy
import matplotlib
from .world import *
from .experiment import *
from .agent_markers import *
from .QuickBundles import *


def adjust_color_brightness(color, amount:float=1):
    import matplotlib.colors as mc
    import colorsys
    try:
        c = mc.cnames[color]
    except:
        c = color
    c = colorsys.rgb_to_hls(*mc.to_rgb(c))
    return colorsys.hls_to_rgb(c[0], 1 - amount * (1 - c[1]), c[2])


class Display:

    def __init__(self,
                 world: World,
                 fig_size: tuple = (10, 10),
                 padding: float = .1,
                 outline: float = .5,
                 show_axes: bool = False,
                 cell_color="white",
                 occlusion_color="black",
                 background_color="white",
                 habitat_color="white",
                 cell_edge_color="lightgray",
                 cell_fill: bool = True,
                 show_cell: bool = True,
                 show_occluded_cell: bool = True,
                 show_habitat: bool = True,
                 habitat_fill: bool = True,
                 occluded_cell_edge_color="lightgray",
                 habitat_edge_color="gray",
                 cell_outline_zorder: int = -8,
                 cell_zorder: int = -7,
                 occluded_cell_zorder: int = -7,
                 habitat_zorder: int = -9,
                 cell_alpha: int = 1,
                 cell_outline_alpha: int = 1,
                 habitat_alpha: int = 1,
                 animated: bool = False,
                 ax=None,
                 fig=None):
        if animated:
            matplotlib.pyplot.ion()
        self.cell_alpha = cell_alpha
        self.show_cell = show_cell
        self.show_occluded_cell = show_occluded_cell
        self.show_habitat = show_habitat
        self.cell_outline_alpha = cell_outline_alpha
        self.habitat_alpha = habitat_alpha
        self.cell_fill = cell_fill
        self.habitat_fill = habitat_fill
        self.agents = dict()
        self.agents_markers = dict()
        self.agents_markers["predator"] = Agent_markers.robot()
        self.agents_markers["prey"] = Agent_markers.mouse()
        self.animated = animated
        self.world = world
        if not fig:
            self.fig = matplotlib.pyplot.figure(figsize=fig_size)
        else:
            self.fig = fig
        if not ax:
            self.ax = self.fig.add_subplot(111)
        else:
            self.ax = ax
        self.ax.axes.xaxis.set_visible(show_axes)
        self.ax.axes.yaxis.set_visible(show_axes)
        self.agents_trajectories = Trajectories()
        self.agents_markers = dict()
        self.cells = []
        self.occlusion_color = occlusion_color
        self.habitat_color = habitat_color
        self.habitat_edge_color = habitat_edge_color
        self.cell_color = cell_color
        self.cell_edge_color = cell_edge_color
        self.occluded_cell_edge_color = occluded_cell_edge_color
        self.xcenter = world.implementation.space.center.x
        self.ycenter = world.implementation.space.center.y
        self.outline = outline
        hsize = world.implementation.space.transformation.size / 2
        pad = hsize * padding

        xmin = self.xcenter - hsize - pad
        xmax = self.xcenter + hsize + pad

        ymin = self.ycenter - hsize - pad
        ymax = self.ycenter + hsize + pad

        self.ax.set_xlim(xmin=xmin, xmax=xmax)
        self.ax.set_ylim(ymin=ymin, ymax=ymax)
        self.ax.set_facecolor(background_color)
        self.habitat_theta = math.radians(0 - world.implementation.space.transformation.rotation)
        self.cells_theta = math.radians(0 - world.implementation.cell_transformation.rotation) + self.habitat_theta
        self.cells_size = world.implementation.cell_transformation.size / 2
        self.habitat_size = hsize
        self.cell_polygons = []
        self.cell_outline_polygons = []
        self.habitat_polygon = None
        self.background_extent = [self.xcenter - hsize, self.xcenter + hsize, self.ycenter - hsize, self.ycenter + hsize]
        self.cell_outline_zorder = cell_outline_zorder
        self.cell_zorder = cell_zorder
        self.occluded_cell_zorder = occluded_cell_zorder
        self.habitat_zorder = habitat_zorder
        self._draw_cells__()
        matplotlib.pyplot.tight_layout()

    def _draw_cells__(self):
        [p.remove() for p in reversed(self.ax.patches)]
        for cell in self.world.cells:
            color = self.occlusion_color if cell.occluded else self.cell_color
            edge_color = self.occluded_cell_edge_color if cell.occluded else self.cell_edge_color
            fill = True if cell.occluded else self.cell_fill
            visible = self.show_occluded_cell if cell.occluded else self.show_cell
            self.cell_outline_polygons.append(self.ax.add_patch(matplotlib.patches.RegularPolygon((cell.location.x, cell.location.y),
                                                                               self.world.configuration.cell_shape.sides,
                                                                               self.cells_size,
                                                                               facecolor=color,
                                                                               edgecolor=edge_color,
                                                                               orientation=self.cells_theta,
                                                                               zorder=self.cell_outline_zorder,
                                                                               linewidth=1,
                                                                               alpha=self.cell_outline_alpha,
                                                                               fill=fill,
                                                                               visible=visible)))
            self.cell_polygons.append(self.ax.add_patch(matplotlib.patches.RegularPolygon((cell.location.x, cell.location.y),
                                                                       self.world.configuration.cell_shape.sides,
                                                                       self.cells_size * self.outline,
                                                                       facecolor=color,
                                                                       orientation=self.cells_theta,
                                                                       zorder=self.cell_zorder,
                                                                       linewidth=0,
                                                                       alpha=self.cell_alpha,
                                                                       fill=fill,
                                                                       visible=visible)))
        self.habitat_polygon = self.ax.add_patch(matplotlib.patches.RegularPolygon((self.xcenter, self.ycenter),
                                                                self.world.implementation.space.shape.sides,
                                                                self.habitat_size,
                                                                facecolor=self.habitat_color,
                                                                edgecolor=self.habitat_edge_color,
                                                                orientation=self.habitat_theta,
                                                                zorder=self.habitat_zorder,
                                                                alpha=self.habitat_alpha,
                                                                fill=self.habitat_fill,
                                                                visible=self.show_habitat))

    def set_occlusions(self, occlusions: Cell_group_builder):
        self.world.set_occlusions(occlusions)
        self._draw_cells__()

    def set_agent_marker(self, agent_name: str, marker: matplotlib.path.Path):
        self.agents_markers[agent_name] = marker

    def add_trajectories(self,
                         trajectories: Trajectories,
                         colors: dict = {"prey": "blue", "predator": "red"},
                         alphas: dict = {"prey": 1, "predator": 1},
                         zorder: int = -6,
                         auto_alpha: bool = False,
                         distance_equalization: bool = False) -> dict:

        def get_segments(locations):
            segments = []
            previous = locations[0]
            for l in locations[1:]:
                segments.append([[previous.x,previous.y],[l.x,l.y]])
                previous=l
            return segments
        lines = {}
        split_trajectories = trajectories.split_by_agent()
        for agent in split_trajectories:
            color = colors[agent]
            alpha = alphas[agent]
            if distance_equalization:
                agent_trajectory = StreamLine(split_trajectories[agent])
            else:
                agent_trajectory = split_trajectories[agent].get("location")
            segments = get_segments(agent_trajectory)

            if auto_alpha:
                step_alphas = [i/len(segments) for i in range(len(segments))]
            else:
                if type(alpha) is list:
                    if len(alpha) == len(segments):
                        step_alphas = alpha
                    else:
                        print ("wrong size for alpha list, assigning 1")
                        step_alphas = [1 for i in range(len(segments))]
                else:
                    step_alphas = [alpha for i in range(len(segments))]

            if type(color) is list:
                if len(color) == len(segments):
                    step_colors = [matplotlib.colors.to_rgb(c) for c in color]
                else:
                    print ("wrong size for color list, expected", len(segments), ", received", len(color), "assigning blue")
                    step_colors = [matplotlib.colors.to_rgb("blue") for i in segments]
            else:
                step_colors = [matplotlib.colors.to_rgb(color) for i in segments]
            color_with_alpha = [c + (a,) for c,a in zip(step_colors, step_alphas)]
            lc = matplotlib.collections.LineCollection(segments, colors=color_with_alpha, lw=3, zorder=zorder)
            lines[agent] = self.ax.add_collection(lc)
        return lines

    def cell(self, cell: Cell = None, cell_id: int = -1, coordinates: Coordinates = None, color=None, outline_color=None, edge_color=None, alpha=None):
        if color is None:
            color = self.cell_color
        if edge_color is None:
            edge_color = self.cell_edge_color
        if cell is None:
            if cell_id == -1:
                if coordinates is None:
                    raise RuntimeError("a cell, cell_id or coordinates must be provided")
                for c in self.world.cells:
                    if c.coordinates == coordinates:
                        cell = c
                        break
                if cell is None:
                    raise RuntimeError("cell coordinates not found")
            else:
                cell = self.world.cells[cell_id]
        if outline_color is None:
            outline_color = color
        if alpha is None:
            alpha = self.cell_alpha
        self.cell_polygons[cell.id].set_facecolor(color)
        self.cell_outline_polygons[cell.id].set_edgecolor(edge_color)
        self.cell_outline_polygons[cell.id].set_facecolor(outline_color)
        self.cell_outline_polygons[cell.id].set_alpha(alpha)

    def heatmap(self, values: list, color_map=matplotlib.pyplot.cm.Reds, value_range: tuple = None) -> None:
        if value_range:
            minv, maxv = value_range
        else:
            minv, maxv = min(values), max(values)

        if (minv == maxv):
            return
        adjusted_values = [(v-minv)/(maxv-minv) for v in values]
        adjusted_cmap = color_map(adjusted_values)
        for cell_id, color in enumerate(adjusted_cmap):
            if not self.world.cells[cell_id].occluded:
                self.cell(cell_id=cell_id, color=color)

    def circle(self, location: Location, radius: float, color, alpha: float = 1.0, zorder: int = -6):
        circle_patch = matplotlib.pyplot.Circle((location.x, location.y), radius, color=color, alpha=alpha, zorder=zorder)
        return self.ax.add_patch(circle_patch)

    def arrow(self, beginning: Location, ending: Location = None, theta: float = 0, dist: float = 0, color="black", head_width: float = .02, alpha: float = 1.0, line_width: float = 0.001, zorder: int = 4, existing_arrow: matplotlib.patches.FancyArrowPatch = None) -> matplotlib.patches.FancyArrowPatch:
        if ending is None:
            ending = beginning.copy().move(theta=theta, dist=dist)
        length = ending - beginning
        if existing_arrow:
            existing_arrow.set_data(x=beginning.x, y=beginning.y, dx=length.x, dy=length.y, head_width=head_width, width=line_width)
            existing_arrow.set_color(color)
            existing_arrow.set_alpha(alpha)
            return existing_arrow
        else:
            new_arrow = self.ax.arrow(beginning.x, beginning.y, length.x, length.y, color=color, head_width=head_width, length_includes_head=True, alpha=alpha, width=line_width, zorder=zorder)
            new_arrow.animated = self.animated
            return new_arrow

    def line(self, beginning: Location, ending: Location = None, theta: float = 0, dist: float = 0, color="black", alpha: float = 1.0, line_width: float = 0.001, zorder: int = -6, existing_line: matplotlib.patches.FancyArrowPatch = None) -> matplotlib.patches.FancyArrowPatch:
        head_width = 0
        if ending is None:
            ending = beginning.copy().move(theta=theta, dist=dist)
        length = ending - beginning
        if existing_line:
            existing_line.set_data(x=beginning.x, y=beginning.y, dx=length.x, dy=length.y, head_width=0, head_length=0, alpha=alpha, width=line_width)
            existing_line.set_color(color)
            existing_line.set_alpha(alpha)
            return existing_line
        else:
            new_arrow = self.ax.arrow(beginning.x, beginning.y, length.x, length.y, color=color, head_width=0, alpha=alpha, head_length=0, length_includes_head=False, width=line_width, zorder=zorder)
            new_arrow.animated = self.animated
            return new_arrow

    def set_background(self, background, alpha: float = 1):
        self.ax.imshow(background, extent=self.background_extent, alpha=alpha)
        self._draw_cells__()

    def add_icon(self, location: Location, icon_name: str = "", icon_file_path: str = "", rotation: float = 0, size: float = .05, zorder: int = -5, alpha:float = 1):
        from matplotlib import image
        from scipy import ndimage
        if icon_name:
            icon = image.imread(Agent_markers.icon_folder() + "/" + icon_name + ".png")
        else:
            if icon_file_path:
                icon = image.imread(icon_file_path)
            else:
                raise "Either icon_name or icon_file_path must be provided"
        rotated_img = ndimage.rotate(icon, -rotation)
        self.ax.imshow(rotated_img, extent=Agent_markers.icon_box(location, size, rotation), alpha=alpha, zorder=zorder)

    def agent(self, step: Step = None, agent_name: str = None, location: Location = None, rotation: float = None, color=None, size: float = 40.0,  zorder: int = -6, marker: matplotlib.path.Path=None):
        if step:
            agent_name = step.agent_name
            location = step.location
            rotation = step.rotation

        if not marker:
            if agent_name in self.agents_markers:
                marker = self.agents_markers[agent_name]
            else:
                if agent_name == "predator":
                    marker = Agent_markers.robot()
                else:
                    marker = Agent_markers.mouse()

        if agent_name not in self.agents:
            self.agents[agent_name], = self.ax.plot(location.x, location.y, marker=marker, c=color, markersize=size, zorder=zorder)

        t = matplotlib.transforms.Affine2D().rotate_deg_around(0, 0, -rotation)
        self.agents[agent_name].set_marker(marker.transformed(t))
        self.agents[agent_name].set_xdata(location.x)
        self.agents[agent_name].set_ydata(location.y)
        self.agents[agent_name].set_color(color)

    def plot_clusters(self, clusters, colors: list=["blue", "red"], show_centroids: bool = True, use_alpha: bool = True, zorder: int = -6):
        for sl in clusters.unclustered:
            self.ax.plot([s.x for s in sl], [s.y for s in sl], c="gray", linewidth=2, alpha=.5)

        for cn, cluster in enumerate(clusters):
            distances = cluster.get_distances()
            max_distance = max(distances)
            if use_alpha:
                alpha_values = [1 if max_distance == 0 else (max_distance - d) / max_distance / 2 for d in distances]
            else:
                alpha_values = [1 for d in distances]
            color = adjust_color_brightness(colors[cn % len(colors)], .8)
            for i, sl in enumerate(cluster):
                self.ax.plot([s.x for s in sl], [s.y for s in sl], c=color, linewidth=2, alpha=alpha_values[i], zorder=zorder)

        if show_centroids:
            for cn, cluster in enumerate(clusters):
                color = adjust_color_brightness(colors[cn % len(colors)], 1.2)
                cluster_size = .005 + .01 * (len(cluster) / clusters.streamline_count())
                for s in cluster.centroid:
                    self.circle(s, radius=cluster_size + .003, color="lightgray", alpha=1, zorder=500)
                for s in cluster.centroid:
                    self.circle(s, radius=cluster_size, color=color, alpha=1, zorder=500)

    def update(self):
        if self.animated:
            self.fig.canvas.draw()
            self.fig.canvas.flush_events()
            matplotlib.pyplot.pause(.001)

def LoadVideo(video_path: str) -> list:
    import cv2
    video_reader = cv2.VideoCapture(video_path)
    frames = []
    while video_reader.isOpened():
        ret, video_frame = video_reader.read()
        if video_frame is None:
            break
        frames.append(video_frame)
    return frames


def CropVideo(frames: list, crop=(-980, None, None, None, None, None)) -> list:
    cropped_frames = []
    for frame in frames:
        cropped_frames.append(frame[crop[0]:crop[1], crop[2]:crop[3], crop[4]:crop[5]])
    return cropped_frames


def EpisodeRepresentation(episode: Episode,
                          world: World,
                          frames: list = None,
                          time_stamps: list = None,
                          colors: dict = {"prey": "blue", "predator": "red"},
                          cols: int = 0,
                          figsize=(10, 10),
                          video_path: str = "",
                          video_frames: list = None,
                          icons: dict = {"prey": "mouse", "predator": "robot"},
                          distance_equalization: int = 30,
                          icon_size: float = .8, **kwargs):
    split = episode.trajectories.split_by_agent()
    if video_path:
        video_frames = CropVideo(LoadVideo(video_path))
    else:
        video_frames = None

    selections = frames if frames else time_stamps
    if cols == 0:
        cols = len(selections)
    rows = math.ceil(len(selections)/cols)
    fig = matplotlib.pyplot.figure(figsize=figsize)
    axs = fig.subplots(rows, cols)
    fig.tight_layout()
    matplotlib.pyplot.subplots_adjust(wspace=0, hspace=0)
    ps = 0
    displays = []
    if frames is None:
        frames = []
        pf = 0
        for ts in time_stamps:
            for i, step in enumerate(episode.trajectories[pf:]):
                if step.time_stamp > ts:
                    frames.append(step.frame)
                    break
                pf = i
            else:
                frames.append(step.frame)
    for i, frame in enumerate(frames):
        ax = axs[int(i/cols), i % cols]
        if video_frames:
            displays.append(Display(world, cell_fill=False, background_color="black", habitat_fill=False, show_cell=False, show_occluded_cell=False, ax=ax))
            displays[-1].background_extent = [0, 1, .0435, .95]
            displays[-1].set_background(video_frames[frame], 1)
        else:
            displays.append(Display(world, cell_fill=True, background_color="black", habitat_fill=True, show_cell=True, show_occluded_cell=True, ax=ax))

        trajectories = episode.trajectories.get_segment(start_frame=ps, end_frame=frame)
        displays[-1].add_trajectories(trajectories, colors=colors, auto_alpha=True, distance_equalization=distance_equalization)
        if not video_frames:
            for agent in split:
                step = split[agent].get_step_by_frame(frame)
                if "." in icons[agent]:
                    displays[-1].add_icon(step.location, icon_file_path=icons[agent], rotation=step.rotation, size=icon_size)
                else:
                    displays[-1].add_icon(step.location, icon_name=icons[agent], rotation=step.rotation, size=icon_size)
        ps = frame

    print (frames)

    return fig, displays