import sys
from src import *


implementation_name = sys.argv[1]
size = float(sys.argv[2])
rotation = float(sys.argv[3])
center_x = float(sys.argv[4])
center_y = float(sys.argv[5])

new_space = Space(Location(center_x, center_y), Shape(6), Transformation(size,rotation))

new_implementation = World_implementation.get_from_name("hexagonal", "canonical")
new_implementation.transform(new_space)
with open("hexagonal."+implementation_name, "w") as f:
    f.write(str(new_implementation))
