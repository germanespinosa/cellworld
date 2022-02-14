from src import *

# occlusions = "10_05"
# world = World.get_from_parameters_names("hexagonal", "cv", occlusions)
#
# graph = Graph.create_connection_graph(world)
#
# graph.to_nxgraph()
#
# print(world.create_cell_group(cell_group_builder=graph[world.cells[2]]))
#

space = Space(Location(5, 5), Shape(6), Transformation(10, 0))


location = Location(10, 10)

new_location = space.scale(location, Scale(-1, -1))

print(new_location)


