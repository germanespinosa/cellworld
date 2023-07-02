import math
import requests
import json
global cellworld_data_base_uri
from datetime import datetime, timedelta
import os

cellworld_data_base_uri = "https://raw.githubusercontent.com/germanespinosa/cellworld_data/master/"
cellworld_cache_folder = os.environ.get("CELLWORLD_CACHE")


def probability_entropy(prob):
    from math import log
    ent = 0
    for p in prob:
        ent += p * log(p)
    return -ent


def normalized_entropy(data, labels=None):
    hist = {}
    if labels:
        for l in labels:
            hist[l] = 0
    for v in data:
        if v in hist:
            hist[v] += 1
        else:
            if labels is None:
                hist[v] = 1
            else:
                raise RuntimeError("data with value " + str(v) + " not present in labels")

    data_count = sum(hist.values())
    data_probability = [l/data_count for l in hist.values()]
    data_entropy = probability_entropy(data_probability)
    if data_entropy == 0:
        return 0
    labels = list(hist.keys())
    label_count = len(labels)
    if label_count == 1:
        return 0
    fair_probability = [1/label_count for l in labels]
    fair_entropy = probability_entropy(fair_probability)
    return data_entropy / fair_entropy


def get_resource(resource_type: str, key0, *argv):
    resource_uri = cellworld_data_base_uri + resource_type + "/" + key0
    file_path = ""
    if cellworld_cache_folder:
        file_path = cellworld_cache_folder + "/" + resource_type + "/" + key0
    for arg in argv:
        resource_uri += "." + arg
        if cellworld_cache_folder:
            file_path += "." + arg
    if cellworld_cache_folder and os.path.exists(file_path):
        content = ""
        with open(file_path, "r") as text_file:
            content = text_file.read()
        return json.loads(content)
    else:
        response = requests.get(resource_uri)
        return json.loads(response.text)


def get_web_json(resource_uri: str):
    response = requests.get(resource_uri)
    return json.loads(response.text)


def to_radians(rotation: float) -> float:
    return rotation / 180 * math.pi


def to_degrees(theta: float) -> float:
    return theta * 180 / math.pi


class Time_out:
    def __init__(self, seconds=1.0):
        self.end_time = datetime.now() + timedelta(seconds=seconds)

    def __bool__(self):
        return self.end_time > datetime.now()


class Timer:
    def __init__(self, seconds: float = 0):
        self.time = seconds
        self.check_point = datetime.now()

    def to_seconds(self) -> float:
        return (datetime.now() - self.check_point).total_seconds()

    def __bool__(self):
        return self.to_seconds() < self.time

    def time_out(self) -> bool:
        return self.to_seconds() > self.time

    def reset(self):
        self.check_point = datetime.now()


def check_type(v, t, m):
    if not isinstance(v, t):
        raise TypeError(m)


def check_types(v, ts, m):
    r = False
    for t in ts:
        if isinstance(v, t):
            r = True
    if not r:
        raise TypeError(m)


def normalize(angle: float) -> float:
    check_type(angle, float, "wrong type for angle")
    while angle < 0:
        angle += 2.0 * math.pi
    while angle > 2 * math.pi:
        angle -= 2.0 * math.pi
    return angle;


def angle_difference(a1: float, a2: float) -> float:
    a1 = normalize(a1)
    a2 = normalize(a2)
    if a1 > a2:
        dif = a1 - a2;
        if dif < math.pi:
            return dif, 1;
        else:
            return a2 + math.pi * 2.0 - a1, -1
    else:
        dif = a2 - a1;
        if dif < math.pi:
            return dif, -1
        else:
            return a1 + math.pi * 2.0 - a2, 1


def angle_between(value: float, lim1: float, lim2: float, inclusive: bool=False) -> bool:
    diff1, dir1 = angle_difference(value, lim1)
    diff2, dir2 = angle_difference(value, lim2)
    if inclusive:
        if diff1 == 0 or diff2 == 0:
            return True
    return (dir1 + dir2) == 0
