import graphs
from time import time
from random import choice
from sys import argv


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

if repr_type == "matrix":
    g = graphs.AdjacencyMatrixGraph(filename)
else:
    g = graphs.AdjacencyVectorsGraph(filename)

for vertex in [10, 20, 30, 40, 50]:
    print(f"--> d(1,{vertex}) = {g.get_distance(1, vertex)}")
    print(f"--> path(1, {vertex}) = {g.get_path(1, vertex)}")

for vertex in [10, 20, 30, 40, 50]:
    print(f"--> eccentricity({vertex}) = {g.get_eccentricity(vertex)}")

vertices = get_random_vertices(g.get_n_vertices(), 100)
start = time()
for vertex in vertices:
    g.get_eccentricity(vertex)
diff_time = time() - start
print("--> Average time of eccentricity computation: {}".format(
    human_time(diff_time / 1000)))

out_fname = "{}_{}_mst.txt".format(filename, repr_type)
start = time()
mst = g.prim(1, out_fname)
diff_time = time() - start
print("--> Time for building MST: {}".format(human_time(diff_time / 1000)))
print("--> MST weights: {}".format(mst[1]))
print("--> MST cost: {}".format(sum(mst[1])))
print("--> Whole MST is saved on file {}".format(out_fname))
