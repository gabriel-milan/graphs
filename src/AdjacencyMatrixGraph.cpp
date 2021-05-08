#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <limits>
#include <mutex>
#include <queue>
#include <stack>
#include <string>
#include <vector>
#include "utils.cpp"
#include "Graph.hpp"

using namespace std;

/*
 * Representing a graph using
 * an adjacency matrix
 */
class AdjacencyMatrixGraph : public Graph
{
public:
  AdjacencyMatrixGraph(string filename)
  {
    // str will be used to store file lines
    string str;
    ifstream graphFile(filename);
    // Read first line in order to get number of vertices
    getline(graphFile, str);
    unsigned vertices = (unsigned)stoi(str);
    this->n_vertices = vertices;
    // Generate the adjacency matrix
    this->adjacencyMatrix = vector<vector<bool>>(vertices);
    for (unsigned i = 0; i < vertices; i++)
      for (unsigned j = 0; j < vertices; j++)
        this->adjacencyMatrix[i].push_back(false);
    // Iterate over file getting edges and adding them to the matrix
    while (getline(graphFile, str))
    {
      vector<string> edgeVerticesStr = vector<string>(2);
      split(str, edgeVerticesStr, ' ');
      vector<int> edgeVertices = vec_stoi<int>(edgeVerticesStr);
      this->add_edge(edgeVertices[0], edgeVertices[1]);
      this->n_edges += 1;
    }
  }

  void add_edge(int vertexA, int vertexB)
  {
    this->adjacencyMatrix.at(vertexA - 1).at(vertexB - 1) = true;
    this->adjacencyMatrix.at(vertexB - 1).at(vertexA - 1) = true;
  }

  void print_matrix()
  {
    for (size_t i = 0; i < this->adjacencyMatrix.size(); i++)
    {
      cout << "| ";
      for (size_t j = 0; j < this->adjacencyMatrix[i].size(); j++)
      {
        cout << this->adjacencyMatrix[i][j] << " ";
      }
      cout << "|" << endl;
    }
  }

  vector<int> get_neighbors(int vertex)
  {
    // Output vector
    vector<int> output;
    // Add every neighbor to output vector
    for (unsigned i = 0; i < this->n_vertices; i++)
      if (this->adjacencyMatrix[vertex][i])
        output.push_back(i);
    // Return
    return output;
  }

protected:
  vector<vector<bool>> adjacencyMatrix;
};
