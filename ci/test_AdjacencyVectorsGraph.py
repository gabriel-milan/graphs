import graphs


def test_adjvectors():
    g = graphs.AdjacencyVectorsGraph("ci/graph.txt")

    assert g.bfs(2, "") == [[1, 1, 4, 4, 1], [1, 0, 2, 2, 1]]
    assert g.dfs(1, "") == [[0, 0, 4, 4, 1], [0, 1, 3, 3, 2]]
    assert g.get_degree_info() == [1.0, 4.0, 2.0, 2.0]
    assert g.get_diameter() == 2
    assert g.get_distance(1, 3) == 2
    g.add_edge(1, 3)
    assert g.get_distance(1, 3) == 1
