import graphs

graph_list = [
    "graph01.txt",
    "graph02.txt",
    "graph03.txt",
    "graph04.txt",
]

bfs_results = [
    [[1, 1, 4, 4, 1], [1, 0, 2, 2, 1], [1, 1, 1, 1, 1]],
    [[2, 1, 1, 1, 5, 3, 3, 2], [2, 0, 1, 1, 3, 2, 2, 2], [1, 1, 1, 1, 1, 1, 1, 1]],
    [[1, 1, -1, -1, -1, -1, 1, -1, -1, -1, -1], [1, 0, 2147483647, 2147483647, 2147483647, 2147483647,
                                                 1, 2147483647, 2147483647, 2147483647, 2147483647], [1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0]],
    [[1, 1, -1, -1, 1, -1, -1, -1, 4, -1, 4, -1], [1, 0, 2147483647, 2147483647, 1, 2147483647,
                                                   2147483647, 2147483647, 2, 2147483647, 2, 2147483647], [1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0]],
]

dfs_results = [
    [[0, 0, 4, 4, 1], [0, 1, 3, 3, 2], [1, 1, 1, 1, 1]],
    [[0, 2, 0, 1, 5, 3, 4, 2], [0, 2, 1, 3, 5, 4, 6, 2], [1, 1, 1, 1, 1, 1, 1, 1]],
    [[0, 0, -1, -1, -1, -1, 1, -1, -1, -1, -1], [0, 1, 2147483647, 2147483647, 2147483647, 2147483647,
                                                 2, 2147483647, 2147483647, 2147483647, 2147483647], [1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0]],
    [[0, 0, -1, -1, 1, -1, -1, -1, 4, -1, 8, -1], [0, 1, 2147483647, 2147483647, 2, 2147483647,
                                                   2147483647, 2147483647, 3, 2147483647, 4, 2147483647], [1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0]],
]

components_results = [
    1,
    1,
    5,
    2,
]

degree_info_results = [
    [1.0, 4.0, 2.0, 2.0],
    [1.0, 5.0, 2.75, 2.0],
    [0.0, 2.0, 1.0909090909090908, 1.0],
    [1.0, 4.0, 2.1666666666666665, 2.0],
]

diameter_results = [
    2,
    4,
    2,
    4,
]

distance_results = [
    2,
    1,
    2147483647,
    2147483647,
]


def test_adjmatrix():
    for i in range(len(graph_list)):
        g = graphs.AdjacencyMatrixGraph(
            "ci/graph_files/{}".format(graph_list[i]))
        assert g.bfs(2, "") == bfs_results[i]
        assert g.dfs(1, "") == dfs_results[i]
        assert g.count_components() == components_results[i]
        assert g.get_degree_info() == degree_info_results[i]
        assert g.get_diameter() == diameter_results[i]
        assert g.get_distance(1, 3) == distance_results[i]
        g.add_edge(1, 3)
        assert g.get_distance(1, 3) == 1


def test_adjvectors():
    for i in range(len(graph_list)):
        g = graphs.AdjacencyVectorsGraph(
            "ci/graph_files/{}".format(graph_list[i]))
        assert g.bfs(2, "") == bfs_results[i]
        assert g.dfs(1, "") == dfs_results[i]
        assert g.count_components() == components_results[i]
        assert g.get_degree_info() == degree_info_results[i]
        assert g.get_diameter() == diameter_results[i]
        assert g.get_distance(1, 3) == distance_results[i]
        g.add_edge(1, 3)
        assert g.get_distance(1, 3) == 1
