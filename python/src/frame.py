from json_cpp import JsonObject, JsonList
from .location import *
from .experiment import Episode


class Frame(JsonObject):

    def __init__(self,
                 frame_number: int = 0,
                 time_stamp: float = 0.0,
                 prey_detection: bool = False,
                 predator_detection: bool = False,
                 prey_location: Location = None,
                 predator_location: Location = None,
                 prey_rotation: float = 0.0,
                 predator_rotation: float = 0.0,
                 prey_data: float = "",
                 predator_data: float = ""):
        self.frame_number = frame_number
        self.time_stamp = time_stamp
        self.prey_detection = prey_detection
        self.predator_detection = predator_detection

        if prey_location is None:
            self.prey_location = Location(0, 0)
        else:
            self.prey_location = prey_location

        if predator_location is None:
            self.predator_location = Location(0, 0)
        else:
            self.predator_location = predator_location

        self.prey_rotation = prey_rotation
        self.predator_rotation = predator_rotation
        self.prey_data = prey_data
        self.predator_data = predator_data
        JsonObject.__init__(self)


class Frame_list(JsonList):

    def __init__(self):
        JsonList.__init__(self, list_type=Frame)

    @classmethod
    def from_episode(cls, episode: Episode):
        frame_list = cls()
        last_frame = max(episode.trajectories.get("frame"))
        prey_trajectories = episode.trajectories.get_agent_trajectory("prey")
        predator_trajectories = episode.trajectories.get_agent_trajectory("predator")
        for frame_number in range(last_frame + 1):
            frame = Frame()
            frame.frame_number = frame_number
            prey_step = prey_trajectories.get_step_by_frame(frame_number)
            if prey_step:
                frame.prey_detection = True
                frame.time_stamp = prey_step.time_stamp
                frame.prey_location = prey_step.location
                frame.prey_rotation = prey_step.rotation
                frame.prey_data = prey_step.data
            else:
                frame.prey_detection = False

            predator_step = predator_trajectories.get_step_by_frame(frame_number)
            if predator_step:
                frame.predator_detection = True
                frame.predator_location = predator_step.location
                frame.predator_rotation = predator_step.rotation
                frame.predator_data = predator_step.data
            else:
                frame.predator_detection = False
            frame_list.append(frame)
        return frame_list
