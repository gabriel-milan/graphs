import os
import graphs

graph_list = [
    "graph05.txt",
    "graph06.txt",
    "graph07.txt",
]

dijkstra_results = [
    [[0.0, 0.0, 0.0, 0.0, 6.0, 3.0, 1.0], [0.0, 1.0, 3.0, 4.0,
                                           6.0, 6.0, 5.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]],
    [[0.0, 0.0, 3.0, 1.0, 0.0, 4.0, 2.0], [0.0, 1.0, 4.0, 3.0,
                                           2.0, 5.0, 6.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]],
    []
]

prim_results = [
    [[0.0, 0.0, 0.0, 2.0, 6.0, 3.0, 1.0], [0.0, 1.0, 3.0, 1.0,
                                           1.0, 2.0, 4.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]],
    [[0.0, 0.0, 3.0, 1.0, 0.0, 3.0, 2.0], [0.0, 1.0, 1.0, 2.0,
                                           2.0, 2.0, 2.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]],
    [[0.0, 0.0, 3.0, 1.0, 0.0, 3.0, 2.0], [0.0, -1.5, 1.0, 2.0,
                                           2.0, 2.0, 2.0], [1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0]],
]

components_results = [
    1,
    1,
    1,
]

degree_info_results = [
    [2.0, 4.0, 3.142857142857143, 3.0],
    [2.0, 5.0, 3.142857142857143, 3.0],
    [2.0, 5.0, 3.142857142857143, 3.0],
]

diameter_results = [
    6.0,
    6.0,
    0.0,
]

eccentricity_results = [
    6.0,
    6.0,
    0.0,
]

distance_results = [
    3.0,
    4.0,
    2147483648.0,
]


def test_adjmatrix():
    for i in range(len(graph_list)):
        g = graphs.AdjacencyMatrixGraph(
            "ci/graph_files/{}".format(graph_list[i]))
        assert g.dijkstra(1, "") == dijkstra_results[i]
        assert g.prim(1, "") == prim_results[i]
        assert g.count_components() == components_results[i]
        assert g.get_degree_info() == degree_info_results[i]
        assert g.get_diameter() == diameter_results[i]
        assert g.get_eccentricity(1) == eccentricity_results[i]
        assert g.get_distance(1, 3) == distance_results[i]
        g.export_to_file("graph.tmp")
        g2 = graphs.AdjacencyMatrixGraph("graph.tmp")
        assert g.get_degree_info() == g2.get_degree_info()
        os.remove("graph.tmp")


def test_adjvectors():
    for i in range(len(graph_list)):
        g = graphs.AdjacencyVectorsGraph(
            "ci/graph_files/{}".format(graph_list[i]))
        assert g.dijkstra(1, "") == dijkstra_results[i]
        assert g.prim(1, "") == prim_results[i]
        assert g.count_components() == components_results[i]
        assert g.get_degree_info() == degree_info_results[i]
        assert g.get_diameter() == diameter_results[i]
        assert g.get_eccentricity(1) == eccentricity_results[i]
        assert g.get_distance(1, 3) == distance_results[i]
        g.export_to_file("graph.tmp")
        g2 = graphs.AdjacencyMatrixGraph("graph.tmp")
        assert g.get_degree_info() == g2.get_degree_info()
        os.remove("graph.tmp")
