#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <limits>
#include <mutex>
#include <queue>
#include <stack>
#include <string>
#include <tuple>
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
    this->adjacencyVectors = vector<vector<tuple<int, float>>>(vertices);
    // Iterate over file getting edges and adding them
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
    // If vertexB is not already connected with vertex A
    if (!contain_tuples_vec(this->adjacencyVectors.at(vertexA - 1), vertexB))
    {
      this->adjacencyVectors.at(vertexA - 1).push_back(make_tuple(vertexB, weight));
      sort(this->adjacencyVectors.at(vertexA - 1).begin(), this->adjacencyVectors.at(vertexA - 1).end());
      this->adjacencyVectors.at(vertexB - 1).push_back(make_tuple(vertexA, weight));
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
        cout << "(" << get<0>(this->adjacencyVectors[i][j]) << ", " << get<1>(this->adjacencyVectors[i][j]) << ")"
             << ", ";
      }
      cout << "]" << endl;
    }
  }

  vector<tuple<int, float>> get_neighbors(int vertex)
  {
    // Output vector
    vector<tuple<int, float>> output;
    // Add every neighbor to output vector
    for (size_t i = 0; i < this->adjacencyVectors[vertex].size(); i++)
    {
      output.push_back(make_tuple(get<0>(this->adjacencyVectors[vertex][i]) - 1, get<1>(this->adjacencyVectors[vertex][i])));
    }
    // Return
    return output;
  }

protected:
  vector<vector<tuple<int, float>>> adjacencyVectors;
};
