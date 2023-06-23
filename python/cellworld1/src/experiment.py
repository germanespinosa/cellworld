from json_cpp import JsonObject, JsonList
from .util import *
from .coordinates import *
from .location import *
from .shape import Space
from .world import World_implementation, World
from .cell import Cell_group_builder
from datetime import datetime


class Step(JsonObject):
    def __init__(self,
                 time_stamp: float = 0.0,
                 agent_name: str = "",
                 frame: int = 0,
                 location: Location = None,
                 rotation: float = 0.0,
                 data: str = ""):
        self.time_stamp = time_stamp
        self.agent_name = agent_name
        self.frame = frame
        if location is None:
            location = Location(0, 0)
        self.location = location
        self.rotation = rotation
        self.data = data


class Velocities(JsonList):
    def __init__(self, iterable=None):
        JsonList.__init__(self, iterable, list_type=float)

    def complementary_filter(self, a: float): #complementary filter
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

    def moving_average_filter(self, window_length: int = 2):
        if window_length <= 0:
            raise ArithmeticError("window length parameter should be > 0")
        filtered = Velocities()
        if len(self) == 0:
            return filtered
        le = len(self) - window_length
        for i in range(len(self)):
            b = i - window_length if i > window_length else 0
            e = i + window_length if i < le else len(self)
            filtered.append(sum(self[b:e])/(e-b))
        return filtered


class Trajectories(JsonList):
    def __init__(self, iterable=None):
        JsonList.__init__(self, iterable, list_type=Step)

    def filter_initial_bad_readings(self):
        first_frame = 1
        for i, step in enumerate(self):
            if step.frame != 0:
                break
            first_frame = i
        del self[0:first_frame-1]

    def get_step_index_by_time_stamp(self, time_stamp: float, exact: bool = False) -> int:
        if len(self) == 1:
            if self[0].time_stamp != time_stamp and exact:
                raise RuntimeError("Time_stamp %i not found", time_stamp)
            else:
                return 0
        m = len(self) // 2
        if self[m].time_stamp == time_stamp:
            return m
        if time_stamp > self[m].time_stamp:
            return m + Trajectories.get_step_index_by_time_stamp(self[m:], time_stamp, exact)
        else:
            return Trajectories.get_step_index_by_time_stamp(self[:m], time_stamp, exact)

    def get_step_by_time_stamp(self, time_stamp: float, exact: bool = False) -> Step:
        step_index = self.get_step_index_by_time_stamp(time_stamp, exact)
        return self[step_index]

    def get_step_index_by_frame(self, frame: int, exact: bool = True) -> int:
        if len(self) == 1:
            if self[0].frame != frame and exact:
                raise RuntimeError("Frame %i not found", frame)
            else:
                return 0
        m = len(self) // 2
        if self[m].frame == frame:
            return m
        if frame > self[m].frame:
            return m + Trajectories.get_step_index_by_frame(self[m:], frame, exact)
        else:
            return Trajectories.get_step_index_by_frame(self[:m], frame, exact)

    def get_step_by_frame(self, frame: int, exact: bool = True) -> Step:
        try:
            step_index = self.get_step_index_by_frame(frame, exact)
            return self[step_index]
        except:
            return None

    def get_segment(self, start_frame=0, end_frame=None, start_time=0, end_time=None):
        segment = Trajectories()
        for step in self:
            if step.frame < start_frame or (end_frame and step.frame > end_frame):
                continue
            if step.time_stamp < start_time or (end_time and step.time_stamp > end_time):
                continue
            segment.append(step)
        return segment

    def get_velocities(self) -> dict:
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

    def get_locations(self) -> dict:
        locations = {}
        for s in self:
            if s.agent_name not in locations:
                locations[s.agent_name] = Location_list()
            locations[s.agent_name].append(s.location)
        return locations

    def get_stops_steps(self, distance_threshold: float = 0.01, stop_time: float = 0.5):
        stops = []
        future = 0
        for current in range(len(self)):
            if current <= future:
                continue
            future = current
            last_future = future
            while future < len(self) and \
                    self[current].location.dist(self[future].location) < distance_threshold:
                #in case of a big break between frames
                if self[future].time_stamp - self[last_future].time_stamp > stop_time:
                    #it repositions itself in the frame before the break and halts
                    future = last_future
                    break
                last_future = future
                future += 1
            if future == len(self):
                break
            if self[current].time_stamp + stop_time < self[future].time_stamp:
                stops.append((self[current], self[future]))
            else:
                future = 0
        return stops

    def get_stops(self, distance_threshold: float = 0.01, stop_time: float = 0.5):
        return [(b.frame, e.frame) for b, e in self.get_stops_steps(distance_threshold, stop_time)]

    def get_filtered_velocities(self, complementary: float = None, outliers: float = None) -> {}:
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

    def split_by_agent(self) -> dict:
        split = {}
        for s in self:
            if s.agent_name not in split:
                split[s.agent_name] = Trajectories()
            split[s.agent_name].append(s)
        return split


class Episode(JsonObject):
    def __init__(self,
                 start_time: datetime = None,
                 time_stamp: float = 0.0,
                 end_time: datetime = None,
                 trajectories: Trajectories = None,
                 captures: JsonList = None,
                 rewards_sequence: Cell_group_builder = None,
                 rewards_time_stamps: JsonList = None):
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
        if captures is None:
            captures = JsonList(list_type=int)
        self.captures = captures
        if rewards_sequence:
            self.rewards_sequence = rewards_sequence
        else:
            self.rewards_sequence = Cell_group_builder()
        if rewards_time_stamps:
            self.rewards_time_stamps = rewards_time_stamps
        else:
            self.rewards_time_stamps = JsonList(list_type=float)

    def clean(self):
        ts = self.trajectories.get_agent_trajectory("predator").where("frame", 0).get("time_stamp")
        if len(ts) > 0:
            min_time_stamp = min(ts)
            new_trajectories = Trajectories()
            for step in self.trajectories:
                if step.frame != 0 or step.agent_name != "predator" or step.time_stamp == min_time_stamp:
                    new_trajectories.append(step)
            self.trajectories = new_trajectories


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
                 episodes: Episode_list = None,
                 episode_count: int = 0,
                 rewards_cells: Cell_group_builder = [],
                 rewards_orientations: JsonList = []):
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
        self.episode_count = episode_count
        if rewards_cells:
            self.rewards_cells = rewards_cells
        else:
            self.rewards_cells = Cell_group_builder()
        if rewards_orientations:
            self.rewards_orientations = rewards_orientations
        else:
            self.rewards_orientations = JsonList(list_type=int)

    def clean_all_episodes (self):
        for episode in self.episodes:
            episode.clean()

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
        self.name = self.start_time.strftime("%Y%m%d_%OH%M_") + self.subject_name + "_" + self.occlusions
        if prefix:
            self.name = prefix + "_" + self.name
        if suffix:
            self.name = self.name + "_" + suffix

    def get_wrong_origin_episodes(self, origin: Coordinates = Coordinates(-20, 0), agent_name: str = "prey") -> JsonList:
        world = World.get_from_parameters_names(self.world_configuration_name, "canonical")
        wrong_origin_episodes = JsonList(list_type=int)
        for i, episode in enumerate(self.episodes):
            agent_trajectory = episode.trajectories.get_agent_trajectory(agent_name)
            if len(agent_trajectory) == 0:
                continue
            episode_origin_location = agent_trajectory[0].location
            episode_origin_index = world.cells.find(episode_origin_location)
            episode_origin_coordinates = world.cells[episode_origin_index].coordinates
            if not episode_origin_coordinates == origin:
                wrong_origin_episodes.append(i)
        return wrong_origin_episodes

    def get_wrong_goal_episodes(self, goal: Coordinates = Coordinates(20, 0), agent_name: str = "prey") -> JsonList:
        world = World.get_from_parameters_names(self.world_configuration_name, "canonical")
        wrong_goal_episodes = JsonList(list_type=int)
        for i, episode in enumerate(self.episodes):
            agent_trajectory = episode.trajectories.get_agent_trajectory(agent_name)
            if len(agent_trajectory) == 0:
                continue
            episode_goal_location = agent_trajectory[-1].location
            episode_goal_index = world.cells.find(episode_goal_location)
            episode_goal_coordinates = world.cells[episode_goal_index].coordinates
            if not episode_goal_coordinates == goal:
                wrong_goal_episodes.append(i)
        return wrong_goal_episodes

    def get_incomplete_episodes(self, threshold_step_count: int = 30, agent_name: str = "prey") -> JsonList:
        incomplete_episodes = JsonList(list_type=int)
        for i, episode in enumerate(self.episodes):
            agent_trajectory = episode.trajectories.get_agent_trajectory(agent_name)
            if len(agent_trajectory) == 0:
                continue
            if len(agent_trajectory) < threshold_step_count:
                incomplete_episodes.append(i)
        return incomplete_episodes

    def get_broken_trajectory_episodes(self, threshold_distance: float = .1, agent_name: str = "prey") -> JsonList:
        broken_trajectory_episodes = JsonList(list_type=int)
        for i, episode in enumerate(self.episodes):
            agent_trajectory = episode.trajectories.get_agent_trajectory(agent_name)
            last_location = None
            for step in agent_trajectory:
                if last_location is not None:
                    if last_location.dist(step.location) > threshold_distance:
                        broken_trajectory_episodes.append(i)
                        break
                last_location = step.location
        return broken_trajectory_episodes

    def remove_episodes(self, episode_list: JsonList) -> None:
        for episode_index in episode_list:
            self.episodes[episode_index].trajectories = Trajectories()
            self.episodes[episode_index].captures = JsonList(list_type=int)
