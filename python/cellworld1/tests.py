from scipy.stats import entropy
from src import *

world = World.get_from_parameters_names("square_small", "canonical", "05_00")
graph = Graph.create_visibility_graph(world)

connection_count = [len(graph._connections[c.id]) for c in world.cells]
print(connection_count)



def histogram(data):
    hist = {}
    for d in data:
        if d in hist:
            hist[d] += 1
        else:
            hist[d] = 1
    min_value = min(hist.keys())
    max_value = max(hist.keys())
    hist = [hist[i] if i in hist else 0 for i in range(min_value,max_value+1)]
    return hist


def entropy(histogram):
    from math import log2
    prob = []
    c = 0
    for h in histogram:
        c += h

    for h in histogram:
        if h > 0:
            prob.append(h/c)
    ent = 0
    for p in prob:
        ent += p * log2(p)
    return -ent


def logb(a, b):
    from math import log
    return log(a) / log(b)


def entropy2(probabilities, base):
    if len(probabilities)<=1:
        return 0
    ent = 0
    for p in probabilities:
        if p>0:
            ent -= p * logb(p, base)
    return ent / logb(len(probabilities), base)


def get_probabilities(data):
    prob = []
    c = 0
    for h in data:
        c += h

    for h in data:
        if h > 0:
            prob.append(h/c)
    return prob


hist = histogram(connection_count)
prob = get_probabilities(hist)
print(prob, entropy2(prob, 2))
