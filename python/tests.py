from src import *
from matplotlib.backend_bases import MouseButton
# occlusions = "10_05"
# world = World.get_from_parameters_names("hexagonal", "cv", occlusions)
#
# graph = Graph.create_connection_graph(world)
#
# graph.to_nxgraph()
#
# print(world.create_cell_group(cell_group_builder=graph[world.cells[2]]))
#

# space = Space(Location(5, 5), Shape(6), Transformation(10, 0))
#
#
# location = Location(10, 10)
#
# new_location = space.scale(location, Scale(-1, -1))
#
# print(new_location)
#
import matplotlib.pyplot as plt
world = World.get_from_parameters_names("hexagonal", "canonical", "10_05")

predator_location = Location(.077, .45)
prey_location = Location(.5, .5)
predator_theta = predator_location.atan(prey_location)
params = Capture_parameters(2, 90, .5)
capture = Capture(params, world)


def on_click(event):
    global prey_location
    global predator_theta
    global predator_location
    if event.button == MouseButton.LEFT:
        prey_location = Location(event.xdata, event.ydata)  # event.x, event.y
    else:
        predator_location = Location(event.xdata, event.ydata)  # event.x, event.y
    predator_theta = predator_location.atan(prey_location)


display = Display(world, fig_size=(9, 8), animated=True)
display.set_agent_marker("predator", Agent_markers.arrow())
display.set_agent_marker("prey", Agent_markers.mouse())
cid = display.fig.canvas.mpl_connect('button_press_event', on_click)
while True:
    predator = predator_location
    if capture.is_captured(predator_location=predator_location, predator_theta=predator_theta, prey_location=prey_location):
        display.agent(location=predator_location, rotation=to_degrees(predator_theta), agent_name="predator", size=20, color="red")
    else:
        display.agent(location=predator_location, rotation=to_degrees(predator_theta), agent_name="predator", size=20, color="blue")

    display.agent(location=prey_location, rotation=0, agent_name="prey", size=20, color="red")
    display.update()
