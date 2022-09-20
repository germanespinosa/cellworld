import networkx as nx
from .cell import Cell_map, Cell_group, Cell, Cell_group_builder
from .world import World


class Graph:

    @staticmethod
    def create_connection_graph(world: World):
        graph = Graph()
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

    def __init__(self):
        self._connections = []

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

    def to_nxgraph (self):
        nxgraph = nx.Graph()
        nxgraph.add_nodes_from(list(range(len(self._connections))))
        for src, conns in enumerate(self._connections):
            for dst in conns:
                nxgraph.add_edge(src, dst)
        return nxgraph

    def __getitem__(self, cell: Cell) -> Cell_group_builder:
        return self._connections[cell.id]

