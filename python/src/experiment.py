from json_cpp import JsonObject, JsonList
from .util import *
from .coordinates import *
from .location import *
from .shape import Space
from .world import World_implementation
from datetime import datetime

class Step(JsonObject):
    def __init__(self,
                 time_stamp: float = 0.0,
                 agent_name: str = "",
                 frame: int = 0,
                 coordinates: Coordinates = None,
                 location: Location = None,
                 rotation: float = 0.0,
                 data: str = ""):
        self.time_stamp = time_stamp
        self.agent_name = agent_name
        self.frame = frame
        if coordinates is None:
            coordinates = Coordinates(0, 0)
        if location is None:
            location = Location(0,0)
        self.coordinates = coordinates
        self.location = location
        self.rotation = rotation
        self.data = data


class Velocities(JsonList):
    def __init__(self, iterable=None):
        JsonList.__init__(self, iterable, list_type=float)

    def complementary_filter(self, a: float):# complementary filter
        check_type(a, float, "wrong type for a")
        if a <= 0 or a >= 1:
            raise ArithmeticError("filter parameter should be > 0 and < 1")
        filtered = Velocities()
        last = self[0]
        for v in self:
            nv = (a * last + (1 - a) * v)
            last = nv
            filtered.append(nv)
        return filtered

    def outliers_filter(self, threshold: float):
        check_type(threshold, float, "wrong type for threshold")
        if threshold <= 0:
            raise ArithmeticError("threshold parameter should be > 0")
        threshold = sum(self)/len(self) * threshold
        filtered = Velocities()
        if len(self) == 0:
            return filtered
        last = self[0]
        for v in self:
            if abs(v-last) <= threshold:
                filtered.append(v)
                last = v
            else:
                filtered.append(last)
        return filtered


class Trajectories(JsonList):
    def __init__(self, iterable=None):
        JsonList.__init__(self, iterable, list_type=Step)

    def get_velocities(self) -> Velocities:
        velocities = {}
        last_locations = {}
        last_time_stamp = {}
        for s in self:
            if s.agent_name not in velocities:
                velocities[s.agent_name] = Velocities()
                velocities[s.agent_name].append(0.0)
            else:
                time_dif = s.time_stamp - last_time_stamp[s.agent_name]
                velocity = s.location.dist(last_locations[s.agent_name]) / time_dif
                velocities[s.agent_name].append(velocity)
            last_locations[s.agent_name] = s.location
            last_time_stamp[s.agent_name] = s.time_stamp
        return velocities

    def get_filtered_velocities(self, complementary: float=None, outliers: float=None) -> Velocities:
        avs = self.get_velocities()
        for agent_name in avs:
            if outliers:
                avs[agent_name] = avs[agent_name].outliers_filter(outliers)
            if complementary:
                avs[agent_name] = avs[agent_name].complementary_filter(complementary)
        return avs

    def get_unique_steps(self):
        unique_steps = Trajectories()
        last_locations = {}
        for s in self:
            if s.agent_name not in last_locations or not last_locations[s.agent_name] == s.location:
                unique_steps.append(s)
            last_locations[s.agent_name] = s.location
        return unique_steps


    def get_agent_names(self) -> list:
        agent_names = []
        for s in self:
            if s.agent_name not in agent_names:
                agent_names.append(s.agent_name)
        return agent_names

    def get_agent_trajectory(self, agent_name: str):
        return self.where("agent_name", agent_name)


class Episode(JsonObject):
    def __init__(self, start_time: datetime = None, time_stamp: float = 0.0, end_time: datetime = None, trajectories: Trajectories = None):
        if not start_time:
            start_time = datetime.now()
        self.start_time = start_time
        self.time_stamp = time_stamp
        if not end_time:
            end_time = datetime.now()
        self.end_time = end_time
        if trajectories is None:
            trajectories = Trajectories()
        self.trajectories = trajectories


class Episode_list(JsonList):
    def __init__(self, iterable=None):
        JsonList.__init__(self, iterable, list_type=Episode)


class Experiment(JsonObject):
    def __init__(self,
                 name: str = "",
                 world_configuration_name: str = "",
                 world_implementation_name: str = "",
                 occlusions: str = "",
                 subject_name: str = "",
                 duration: int = 0,
                 start_time: datetime = None,
                 episodes: Episode_list = None):
        self.name = name
        self.world_configuration_name = world_configuration_name
        self.world_implementation_name = world_implementation_name
        self.occlusions = occlusions
        self.subject_name = subject_name
        self.duration = duration
        if start_time is None:
            start_time = datetime.now()
        self.start_time = start_time
        if episodes is None:
            episodes = Episode_list()
        self.episodes = episodes

    @staticmethod
    def get_from_file(file_name: str):
        e = Experiment.parse(open(file_name).read())
        check_type(e, Experiment, "")
        return e

    @staticmethod
    def get_from_url(url: str):
        e = Experiment.parse(get_web_json(resource_uri=url))
        check_type(e, Experiment, "")
        return e

    def transform(self, dst_space_name: str):
        check_type(dst_space_name, str, "incorrect type for dst_space")

        dst_space = World_implementation.get_from_name(self.world_configuration_name, dst_space_name).space

        src_space = World_implementation.get_from_name(self.world_configuration_name, self.world_implementation_name).space
        for episode in self.episodes:
            for step in episode.trajectories:
                step.location = Space.transform_to(step.location, src_space, dst_space)
        self.world_implementation_name = dst_space_name

    def set_name(self, prefix: str = None, suffix: str = None):
        self.name = self.start_time.strftime("%Y%m%d_%OH%M_") + self.subject_name + "_" + self.occlusions;
        if prefix:
            self.name = prefix + "_" + self.name
        if suffix:
            self.name = self.name + "_" + suffix
