import math

from scipy.stats import entropy


def probability_entropy(prob, base: float = math.e):
    from math import log
    ent = 0
    for p in prob:
        if p > 0:
            ent += p * log(p, base)
    return -ent


def fair_probability(symbol_count: int):
    return [1 / symbol_count for i in range(symbol_count)]


def fair_entropy(symbol_count: int, base: float = math.e):
    return probability_entropy(fair_probability(symbol_count), base=base)


def histogram(data, labels=None, closest: bool = False):
    hist = {}
    if closest and (labels is None or len(labels) == 0):
        raise RuntimeError("if closest is used then labels must be specified")

    if labels:
        for l in labels:
            hist[l] = 0
    for v in data:
        if closest:
            min_distance = abs(v-labels[0])
            i = labels[0]
            for l in labels[1:]:
                distance = abs(v-labels[0])
                if min_distance > distance:
                    i = l
                    min_distance = distance
        i = v
        if i in hist:
            hist[i] += 1
        else:
            if labels is None:
                hist[i] = 1
            else:
                raise RuntimeError("data with value " + str(i) + " not present in labels")
    return list(hist.keys()), list(hist.values())


def entropy(data, labels=None, base: float = math.e):
    labels, values = histogram(data, labels)
    data_count = sum(values)
    data_probability = [v/data_count for v in values]
    data_entropy = probability_entropy(data_probability, base)
    return data_entropy


def normalized_entropy(data, labels=None, base: float = math.e):
    labels, values = histogram(data, labels)
    data_count = sum(values)
    data_probability = [v/data_count for v in values]
    data_entropy = probability_entropy(data_probability, base=base)
    if data_entropy == 0:
        return 0
    label_count = len(labels)
    if label_count == 1:
        return 0
    fair_entropy = probability_entropy(fair_probability(label_count), base=base)
    return data_entropy / fair_entropy

