from matplotlib import pyplot as plt
import cellworld_py as cp
import math


wc = cp.World_configuration.get_from_name("hexagonal")
wi = cp.World_implementation.get_from_name("hexagonal", "canonical")

center = cp.Location()
center.x = .5
center.y = .5
x_change = 1 / (41 + math.tan(60*math.pi/180))
y_change = 0.0405

wi.cell_transformation.size = 0.0405 * 4 / 3

for i, c in enumerate(wc.cell_coordinates):
    wi.cell_locations[i].x = center.x + x_change * c.x
    wi.cell_locations[i].y = center.y + y_change * c.y

wi.space_transformation.size = 1

center = cp.Location()
center.x = 0
center.y = 0

center_coordinates = cp.Coordinates()
center_coordinates.x = 0
center_coordinates.y = 0


src_space = cp.Space(center=cp.Location(0, 0), shape=cp.Shape(6), transformation=cp.Transformation(21.0 + 1.0 / 3.0, 30))

wi2 = cp.World_implementation.create(wc, center, center_coordinates, relative_locations_transformations=(6, -90, 1))

wi2.space = src_space
wi2.cell_transformation.size = 1.15470053837925
wi2.cell_transformation.rotation = 0


m = cp.Cell_map(coordinates_list=wc.cell_coordinates)


for i in range(10):
    dst_space = cp.Space(center=cp.Location(0, 0), shape=cp.Shape(6), transformation=cp.Transformation(10 * (i+1) , 30 + i * 10))
    wi2.transform(dst_space)
    w = cp.World.get_from_parameters(wc, wi2)
    w.cells[m[cp.Coordinates(-20, 0)]].occluded = True
    d = cp.Display(w)
    plt.show()

