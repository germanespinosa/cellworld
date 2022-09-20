import requests
from json_cpp import *

uri = "https://raw.githubusercontent.com/germanespinosa/cellworld_data/master/cell_group/hexagonal.05_05.occlusions"
#req = requests.get(uri)
a = JsonList(list_type=int).load_from_url(uri)

# from src import *
# from time import sleep
# from json_cpp import JsonObject, JsonList
#
#
# progress = JsonObject.load_from_file("belief_state.json")
#
# options = get_resource("graph", "hexagonal", "20_05", "options")
# lppos = get_resource("cell_group", "hexagonal", "20_05", "lppo")
# world = World.get_from_parameters_names("hexagonal", "canonical", "21_05")
# world = World.get_from_parameters_names("hexagonal", "mice", "21_05" )
# ws = World_statistics.get_from_parameters_names("hexagonal", "21_05")
#
#
# def on_click(e):
#     cell_id = world.cells.find(Location(e.xdata, e.ydata))
#     print(world.cells[cell_id])
#     for cell in world.cells:
#         if not cell.occluded:
#             display.cell(cell=cell, color="white")
#     for lppo in lppos:
#         display.cell(cell_id=lppo, color="red", outline_color="blue")
#     for option in options[cell_id]:
#         display.cell(cell_id=option, color="green")
#     display.cell(cell_id=cell_id, color="red")
#
#
# display = Display(world, animated=True)
# cid1 = display.fig.canvas.mpl_connect('button_press_event', on_click)
# for lppo in lppos:
#     display.cell(cell_id=lppo, color="red", outline_color="blue")
#
# l1 = Location(.5, .5)
# l2 = Location(.6, .6)
# arrow = display.arrow(l1, ending=l2, color="orange")
#
# display.update()
# sleep(1)
#
# for i in range(10):
#     l2 += Location(.01,.01)
#     display.arrow(l1, ending=l2, color="orange", existing_arrow=arrow)
#     display.update()
#     sleep(.1)

# display.heatmap(ws.visual_connections)
# while True:
#     display.update()
#     sleep(1)
#
# from matplotlib.backend_bases import MouseButton
# # occlusions = "10_05"
# # world = World.get_from_parameters_names("hexagonal", "cv", occlusions)
# #
# # graph = Graph.create_connection_graph(world)
# #
# # graph.to_nxgraph()
# #
# # print(world.create_cell_group(cell_group_builder=graph[world.cells[2]]))
# #
#
# # space = Space(Location(5, 5), Shape(6), Transformation(10, 0))
# #
# #
# # location = Location(10, 10)
# #
# # new_location = space.scale(location, Scale(-1, -1))
# #
# # print(new_location)
# #
# # import matplotlib.pyplot as plt
# # world = World.get_from_parameters_names("hexagonal", "canonical", "10_05")
# #
# # predator_location = Location(.077, .45)
# # prey_location = Location(.5, .5)
# # predator_theta = predator_location.atan(prey_location)
# # params = Capture_parameters(2, 90, .5)
# # capture = Capture(params, world)
# #
# #
# # def on_click(event):
# #     global prey_location
# #     global predator_theta
# #     global predator_location
# #     if event.button == MouseButton.LEFT:
# #         prey_location = Location(event.xdata, event.ydata)  # event.x, event.y
# #     else:
# #         predator_location = Location(event.xdata, event.ydata)  # event.x, event.y
# #     predator_theta = predator_location.atan(prey_location)
# #
# #
# # display = Display(world, fig_size=(9, 8), animated=True)
# # display.set_agent_marker("predator", Agent_markers.arrow())
# # display.set_agent_marker("prey", Agent_markers.mouse())
# # cid = display.fig.canvas.mpl_connect('button_press_event', on_click)
# # t = Timer(3)
# # i = 0
# # while True:
# #     if t.time_out():
# #         display.set_occlusions(Cell_group_builder.get_from_name("hexagonal", "10_0" + str(i), "occlusions"))
# #         i += 1
# #         t.reset()
# #     predator = predator_location
# #     if capture.is_captured(predator_location=predator_location, predator_theta=predator_theta, prey_location=prey_location):
# #         display.agent(location=predator_location, rotation=to_degrees(predator_theta), agent_name="predator", size=20, color="red")
# #     else:
# #         display.agent(location=predator_location, rotation=to_degrees(predator_theta), agent_name="predator", size=20, color="blue")
# #
# #     display.agent(location=prey_location, rotation=0, agent_name="prey", size=20, color="red")
# #     display.update()
# #
# #
#
# #
# # print(-Coordinates(1,1))
# #
# # from time import sleep
# #
# # world = World.get_from_parameters_names("hexagonal", "canonical", "20_05")
# #
# # def show_heatmap(counters: list, upper_limit: int = 90, occlusions:str = "20_05"):
# #     w = World.get_from_parameters_names("hexagonal", "canonical", occlusions)
# #     d = Display(w, animated=True)
# #     d.fig.suptitle("World " + occlusions, fontsize=50)
# #     nv = [min([ve,upper_limit])/upper_limit for ve in counters]
# #     cmap = plt.cm.Reds(nv)
# #     for i, counter in enumerate(counters):
# #         if not w.cells[i].occluded:
# #             d.cell(cell_id=i, color=cmap[i])
# #     while True:
# #         d.update()
# #         sleep(1)
# #         pass
# #
# #
# #
# # centrality_derivative = world.get_centrality_derivative_product(depth=1)
# # show_heatmap(centrality_derivative, upper_limit=.0001)
#
#
#
#
#
# #
# # plist = Polygon_list.get_polygons(world.cells.occluded_cells().get("location"), world.configuration.cell_shape.sides, world.implementation.cell_transformation.size/2, world.implementation.cell_transformation.rotation)
# #
# # visibility = Location_visibility(plist)
# #
# # vc = visibility.visible_cells(Location(.5, .5), world.cells)
# #
# # d = Display(world, animated=True)
# # d.cell(cell_id=1, color="green")
# #
# # from time import sleep
# #
# # while(True):
# #     d.update()
# #     sleep(1)
# #     pass
#
#
# # e = Experiment.get_from_file("test_experiment.json")
# #
# # to_clean = e.get_wrong_origin_episodes() + e.get_wrong_goal_episodes() + e.get_incomplete_episodes() + e.get_broken_trajectory_episodes()
# #
# # print(" e.get_wrong_origin_episodes()",  e.get_wrong_origin_episodes())
# # print(" e.get_wrong_goal_episodes()",  e.get_wrong_goal_episodes())
# # print(" e.get_incomplete_episodes()",  e.get_incomplete_episodes())
# # print(" e.get_broken_trajectory_episodes()",  e.get_broken_trajectory_episodes())
# #
# # print(set(to_clean))
# #
# # print(to_clean)
# #
# # e.remove_episodes(to_clean)
# #
# # to_clean = e.get_wrong_origin_episodes() + e.get_wrong_goal_episodes() + e.get_incomplete_episodes() + e.get_broken_trajectory_episodes()
# #
# # print(to_clean)
#
# # e = Experiment.get_from_file("test_experiment.json")
# #
# # for i, episode in enumerate(e.episodes):
# #     trajectory = episode.trajectories.get_agent_trajectory("prey")
# #     print ("episode", i, trajectory.get_stops())
#
#
# # world = World.get_from_parameters_names("hexagonal", "canonical", "21_05")
# # print(world.get_connection_pattern_pairs())

#
