import graphs
from os import getpid
from time import time
from random import choice
from sys import argv, exit
from psutil import Process


def human_bytes(bytes, suffix="B"):
    for unit in ["", "K", "M", "G", "T", "P", "E", "Z"]:
        if abs(bytes) < 1024:
            return "%3.1f%s%s" % (bytes, unit, suffix)
        bytes /= 1024
    return "%.1f%s%s" % (bytes, 'Y', suffix)


def human_time(seconds):
    hours = seconds // 3600
    minutes = seconds // 60 % 60
    seconds = seconds % 60
    if hours > 0:
        return "{}h {}min {:.2f}s".format(hours, minutes, seconds)
    elif minutes > 0:
        return "{}min {:.2f}s".format(minutes, seconds)
    else:
        return "{:.2f}s".format(seconds)


def get_random_vertex(n_vertices):
    return choice(list(range(1, n_vertices + 1)))


def get_random_vertices(n_vertices, n):
    output = []
    for _ in range(n):
        output.append(get_random_vertex(n_vertices))
    return output


if len(argv) != 3:
    raise ValueError(
        "Arguments must be graph filename and representation type (matrix or vectors)")

filename = argv[1]
repr_type = argv[2]

if repr_type not in ["matrix", "vectors"]:
    raise ValueError("Representation type must be matrix or vectors")

print("Graph {}, representation using {}".format(filename, repr_type))

start_mem = Process(getpid()).memory_info().rss
if repr_type == "matrix":
    g = graphs.AdjacencyMatrixGraph(filename)
else:
    g = graphs.AdjacencyVectorsGraph(filename)
diff_mem = Process(getpid()).memory_info().rss - start_mem
print("Memory used by graph is {}".format(human_bytes(diff_mem)))

vertices = get_random_vertices(g.get_n_vertices(), 1000)
start = time()
for vertex in vertices:
    g.bfs(vertex, "")
diff_time = time() - start
print("Average time of a BFS: {}".format(human_time(diff_time / 1000)))

start = time()
for vertex in vertices:
    g.dfs(vertex, "")
diff_time = time() - start
print("Average time of a DFS: {}".format(human_time(diff_time / 1000)))

if repr_type == "matrix":
    print("Exiting for I'm of type matrix")
    exit(0)

for start_vertex in [1, 2, 3]:
    for find_vertex in [10, 20, 30]:
        bfs = g.bfs(start_vertex, "")
        print("Parent of {} starting BFS on {} is {}".format(
            find_vertex, start_vertex, bfs[0][find_vertex - 1] + 1))
        dfs = g.dfs(start_vertex, "")
        print("Parent of {} starting DFS on {} is {}".format(
            find_vertex, start_vertex, dfs[0][find_vertex - 1] + 1))

for vertices_pair in [(10, 20), (10, 30), (20, 30)]:
    print("Distance between {} is {}".format(vertices_pair,
                                             g.get_distance(vertices_pair[0], vertices_pair[1])))

components = g.get_components()
max_size = 0
min_size = float("inf")
for c in components:
    if len(c) > max_size:
        max_size = len(c)
    if len(c) < min_size:
        min_size = len(c)
print("There are {} components".format(len(components)))
print("Largest component has size {}".format(max_size))
print("Smallest components size is {}".format(min_size))

start = time()
diameter = g.get_diameter()
diff_time = time() - start
print("Diameter of the graph is {}, it took me {} to compute it".format(
    diameter, human_time(diff_time)))
