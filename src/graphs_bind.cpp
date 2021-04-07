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
      .def(py::init<const char *>())
      .def("add_edge", &AdjacencyMatrixGraph::add_edge)
      .def("bfs", &AdjacencyMatrixGraph::bfs)
      .def("get_distance", &AdjacencyMatrixGraph::get_distance)
      .def("get_diameter", &AdjacencyMatrixGraph::get_diameter)
      .def("print_matrix", &AdjacencyMatrixGraph::print_matrix);

  py::class_<AdjacencyVectorsGraph>(m, "AdjacencyVectorsGraph")
      .def(py::init<const char *>())
      .def("add_edge", &AdjacencyVectorsGraph::add_edge)
      .def("bfs", &AdjacencyVectorsGraph::bfs)
      .def("get_distance", &AdjacencyVectorsGraph::get_distance)
      .def("get_diameter", &AdjacencyVectorsGraph::get_diameter)
      .def("print_vectors", &AdjacencyVectorsGraph::print_vectors);
}