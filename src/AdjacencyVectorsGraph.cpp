#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
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
 * a adjacency vectors
 */
class AdjacencyVectorsGraph : public Graph
{
public:
  AdjacencyVectorsGraph(string filename)
  {
    // str will be used to store file lines
    string str;
    ifstream graphFile(filename);
    // Read first line in order to get number of vertices
    getline(graphFile, str);
    unsigned vertices = (unsigned)stoi(str);
    this->n_vertices = vertices;
    // Generate adjacency vectors
    this->adjacencyVectors = vector<vector<int>>(vertices);
    // Iterate over file getting edges and adding them
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
    // If vertexB is not already connected with vertex A
    if (std::find(this->adjacencyVectors.at(vertexA - 1).begin(), this->adjacencyVectors.at(vertexA - 1).end(), vertexB) == this->adjacencyVectors.at(vertexA - 1).end())
    {
      this->n_edges++;
      this->adjacencyVectors.at(vertexA - 1).push_back(vertexB);
      sort(this->adjacencyVectors.at(vertexA - 1).begin(), this->adjacencyVectors.at(vertexA - 1).end());
      this->adjacencyVectors.at(vertexB - 1).push_back(vertexA);
      sort(this->adjacencyVectors.at(vertexB - 1).begin(), this->adjacencyVectors.at(vertexB - 1).end());
    }
  }

  void print_vectors()
  {
    for (size_t i = 0; i < this->adjacencyVectors.size(); i++)
    {
      cout << i + 1 << " => [";
      for (size_t j = 0; j < this->adjacencyVectors[i].size(); j++)
      {
        cout << this->adjacencyVectors[i][j] << ", ";
      }
      cout << "]" << endl;
    }
  }

  vector<int> get_neighbors(int vertex)
  {
    // Output vector
    vector<int> output;
    // Add every neighbor to output vector
    for (size_t i = 0; i < this->adjacencyVectors[vertex].size(); i++)
      output.push_back(this->adjacencyVectors[vertex][i] - 1);
    // Return
    return output;
  }

protected:
  vector<vector<int>> adjacencyVectors;
};
