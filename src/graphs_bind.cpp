#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "AdjacencyMatrixGraph.cpp"
#include "AdjacencyVectorsGraph.cpp"
#include "version.h"

namespace py = pybind11;

PYBIND11_MODULE(graphs, m)
{
    m.attr("__version__") = __version__;

    py::class_<Graph>(m, "Graph")
        .def(py::init<>());

    py::class_<AdjacencyMatrixGraph>(m, "AdjacencyMatrixGraph")
        .def(py::init<string>())
        .def(
            "add_edge", [](AdjacencyMatrixGraph &self, int vertexA, int vertexB, float weight) {
                return self.add_edge(vertexA, vertexB, weight);
            },
            py::arg("vertexA"), py::arg("vertexB"), py::arg("weight") = (float)1.0)
        .def("bfs", &AdjacencyMatrixGraph::bfs)
        .def("dfs", &AdjacencyMatrixGraph::dfs)
        .def("dijkstra", &AdjacencyMatrixGraph::dijkstra)
        .def("prim", &AdjacencyMatrixGraph::prim)
        .def("info_to_file", &AdjacencyMatrixGraph::info_to_file)
        .def("export_to_file", &AdjacencyMatrixGraph::export_to_file)
        .def("count_components", &AdjacencyMatrixGraph::count_components)
        .def("get_components", &AdjacencyMatrixGraph::get_components)
        .def("get_degree_info", &AdjacencyMatrixGraph::get_degree_info)
        .def("get_all_distances", &AdjacencyMatrixGraph::get_all_distances)
        .def("get_distance", &AdjacencyMatrixGraph::get_distance)
        .def("get_all_paths", &AdjacencyMatrixGraph::get_all_paths)
        .def("get_path", &AdjacencyMatrixGraph::get_path)
        .def("get_eccentricity", &AdjacencyMatrixGraph::get_eccentricity)
        .def("get_diameter", &AdjacencyMatrixGraph::get_diameter)
        .def("get_n_edges", &AdjacencyMatrixGraph::get_n_edges)
        .def("get_n_vertices", &AdjacencyMatrixGraph::get_n_vertices)
        .def("print_matrix", &AdjacencyMatrixGraph::print_matrix);

    py::class_<AdjacencyVectorsGraph>(m, "AdjacencyVectorsGraph")
        .def(py::init<string>())
        .def(
            "add_edge", [](AdjacencyVectorsGraph &self, int vertexA, int vertexB, float weight) {
                return self.add_edge(vertexA, vertexB, weight);
            },
            py::arg("vertexA"), py::arg("vertexB"), py::arg("weight") = (float)1.0)
        .def("bfs", &AdjacencyVectorsGraph::bfs)
        .def("dfs", &AdjacencyVectorsGraph::dfs)
        .def("dijkstra", &AdjacencyVectorsGraph::dijkstra)
        .def("prim", &AdjacencyVectorsGraph::prim)
        .def("info_to_file", &AdjacencyVectorsGraph::info_to_file)
        .def("export_to_file", &AdjacencyVectorsGraph::export_to_file)
        .def("count_components", &AdjacencyVectorsGraph::count_components)
        .def("get_components", &AdjacencyVectorsGraph::get_components)
        .def("get_degree_info", &AdjacencyVectorsGraph::get_degree_info)
        .def("get_all_distances", &AdjacencyVectorsGraph::get_all_distances)
        .def("get_distance", &AdjacencyVectorsGraph::get_distance)
        .def("get_all_paths", &AdjacencyVectorsGraph::get_all_paths)
        .def("get_path", &AdjacencyVectorsGraph::get_path)
        .def("get_eccentricity", &AdjacencyVectorsGraph::get_eccentricity)
        .def("get_diameter", &AdjacencyVectorsGraph::get_diameter)
        .def("get_n_edges", &AdjacencyVectorsGraph::get_n_edges)
        .def("get_n_vertices", &AdjacencyVectorsGraph::get_n_vertices)
        .def("print_vectors", &AdjacencyVectorsGraph::print_vectors);
}
