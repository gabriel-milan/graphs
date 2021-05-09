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
    this->adjacencyMatrix = vector<vector<float>>(vertices);
    for (unsigned i = 0; i < vertices; i++)
      for (unsigned j = 0; j < vertices; j++)
        this->adjacencyMatrix[i].push_back(numeric_limits<float>::epsilon());
    // Iterate over file getting edges and adding them to the matrix
    while (getline(graphFile, str))
    {
      vector<string> edgeVerticesStr;
      split(str, edgeVerticesStr, ' ');
      if (edgeVerticesStr.size() == 3)
      {
        this->is_weighted = true;
        vector<float> edgeVertices = vec_stof<float>(edgeVerticesStr);
        if (edgeVertices[2] < 0)
          this->has_negative_weight = true;
        this->add_edge((int)edgeVertices[0], (int)edgeVertices[1], edgeVertices[2]);
      }
      else
      {
        vector<int> edgeVertices = vec_stoi<int>(edgeVerticesStr);
        this->add_edge(edgeVertices[0], edgeVertices[1]);
      }
      this->n_edges += 1;
    }
  }

  void add_edge(int vertexA, int vertexB, float weight = 1.0)
  {
    this->adjacencyMatrix.at(vertexA - 1).at(vertexB - 1) = weight;
    this->adjacencyMatrix.at(vertexB - 1).at(vertexA - 1) = weight;
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

  vector<tuple<int, float>> get_neighbors(int vertex)
  {
    // Output vector
    vector<tuple<int, float>> output;
    // Add every neighbor to output vector
    for (unsigned i = 0; i < this->n_vertices; i++)
      if (this->adjacencyMatrix[vertex][i] != numeric_limits<float>::epsilon())
        output.push_back(make_tuple(i, this->adjacencyMatrix[vertex][i]));
    // Return
    return output;
  }

protected:
  vector<vector<float>> adjacencyMatrix;
};
