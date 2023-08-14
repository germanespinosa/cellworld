import networkx as nx
from .info import normalized_entropy, entropy
from .cell import Cell_map, Cell_group, Cell, Cell_group_builder
from .world import World
from .visibility import Location_visibility
from json_cpp import JsonList


class Graph_builder(JsonList):
    def __init__(self):
        JsonList.__init__(self, list_type=Cell_group_builder)


class Graph:

    @staticmethod
    def create_visibility_graph(world: World, include_self_reference=False):
        graph = Graph(cells=world.cells)
        location_visibility = Location_visibility.from_world(world)
        for src in world.cells:
            if src.occluded:
                continue
            for dst in world.cells:
                if dst == src and not include_self_reference:
                    continue
                if graph.is_connected(src, dst):
                    continue
                if location_visibility.is_visible(src.location, dst.location):
                    graph.connect(src, dst)
        return graph

    @staticmethod
    def create_connection_graph(world: World):
        graph = Graph(cells=world.cells)
        map = Cell_map(world.configuration.cell_coordinates)
        for cell in world.cells:
            src = cell
            src_coord = cell.coordinates
            if not cell.occluded:
                for conn in world.configuration.connection_pattern:
                    dst_coord = src_coord + conn
                    dst_index = map[dst_coord]
                    if dst_index >= 0:
                        dst = world.cells[dst_index]
                        if not dst.occluded:
                            graph.connect(src, dst)
        return graph

    def __init__(self, builder: Graph_builder = None, cells: Cell_group = None):
        if builder:
            self._connections = builder
        else:
            self._connections = Graph_builder()
        self.cells = cells

    def connect(self, src: Cell, dst: Cell, bi=True):
        while src.id >= len(self._connections):
            self._connections.append(Cell_group_builder())
        while dst.id >= len(self._connections):
            self._connections.append(Cell_group_builder())

        if not dst.id in self._connections[src.id]:
            self._connections[src.id].append(dst.id)
        if bi and not src.id in self._connections[dst.id]:
            self._connections[dst.id].append(src.id)

    def is_connected(self, src: Cell, dst: Cell) -> bool:
        if src.id == dst.id:
            return True
        if src.id >= len(self._connections):
            return False
        if dst.id >= len(self._connections[src.id]):
            return False
        pending = [src.id]
        visited = []
        while pending:
            next_id = pending[0]
            if dst.id in self._connections[next_id]:
                return True
            visited.append(next_id)
            pending = pending[1:]
            pending += [c for c in self._connections[next_id] if not c in visited and not c in pending]
        return False

    def to_nxgraph (self, include_occluded=False):
        nxgraph = nx.Graph()
        if self.cells:
            for cell in self.cells:
                if include_occluded or not cell.occluded:
                    nxgraph.add_node(cell.id, pos=(cell.location.x, cell.location.y), color="black" if cell.occluded else "white")
        else:
            nxgraph.add_nodes_from(list(range(len(self._connections))))
        for src, conns in enumerate(self._connections):
            for dst in conns:
                nxgraph.add_edge(src, dst)
        return nxgraph

    def __getitem__(self, cell: Cell) -> Cell_group_builder:
        return self._connections[cell.id]

    def get_builder(self):
        return self._connections

    def vertex_degrees(self, cell: Cell) -> int:
        return len(self[cell])

    def complexity(self, normalized: bool = False):
        counters = [len(c) for c in self._connections if c]
        if normalized:
            return normalized_entropy(counters, labels=list(range(1, len(self._connections))))
        else:
            return entropy(counters, labels=list(range(1, len(self._connections))))
