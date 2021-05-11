import web_resources as wr
import networkx as nx

class Graph:
    def __init__(self, graph_name=None):
        if graph_name:
            self._connections = wr.get_resource("graph", graph_name)
        else:
            self._connections = []

    def connect(self, src, dst, bi=True):
        if src >= len(self._connections):
            self._connections += [[] for x in range(src-len(self._connections)+1)]
        if not self.is_connected(src, dst):
            self._connections[src].append(dst)
        if bi and not self.is_connected(dst, src):
            self._connections[dst].append(src)

    def is_connected(self, src, dst):
        if src >= len(self._connections):
            return False
        return dst in self._connections[src]

    def to_nxgraph (self):
        nxgraph = nx.Graph()
        nxgraph.add_nodes_from(list(range(len(self._connections))))
        for src, conns in enumerate(self._connections):
            for dst in conns:
                nxgraph.add_edge(src, dst)
        return nxgraph