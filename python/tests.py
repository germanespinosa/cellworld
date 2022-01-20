from src import *
#
# occlusions = "10_05"
#
# world = World.get_from_parameters_names("hexagonal", "cv", occlusions)


e = Experiment()


e.occlusions = "10_05"
e.start_time = datetime.now()
e.subject_name = "subject"
e.set_name("prefix", "suffix")

print (e)