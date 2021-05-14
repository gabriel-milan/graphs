import graphs

def parse_collaborators (fname):
    d = {}
    with open(fname, "r") as f:
        for line in f.readlines():
            key, name = line.split(",")
            d[name] = key
        f.close()
    return d

collaborators = parse_collaborators("data/rede_colaboracao_vertices.txt")
g = graphs.AdjacencyVectorsGraph("data/rede_colaboracao.txt")
root_name = "Edsger W. Dijkstra\n"
root = int(collaborators[root_name])
researchers = ["Alan M. Turing\n", "J. B. Kruskal\n", "Jon M. Kleinberg\n", "Éva Tardos\n", "Daniel R. Figueiredo\n"]

for r in researchers:
    b = int(collaborators[r])
    print("Distance between {} and {}: {}".format(root_name[:-1], r[:-1], g.get_distance(root, b)))
