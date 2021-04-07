import graphs


def test_adjmatrix():
    g = graphs.AdjacencyMatrixGraph("ci/graph.txt")

    assert g.bfs(2, "") == [[1, 1, 4, 4, 1], [1, 0, 2, 2, 1]]
    assert g.get_diameter() == 2
    assert g.get_distance(1, 3) == 2
    g.add_edge(1, 3)
    assert g.get_distance(1, 3) == 1
