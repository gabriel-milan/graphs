#pragma once

#include <iostream>
#include <fstream>
#include <queue>
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
  AdjacencyMatrixGraph(const char *filename)
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
      vector<unsigned> edgeVertices = vec_stoi<unsigned>(edgeVerticesStr);
      this->add_edge(edgeVertices[0], edgeVertices[1]);
    }
  }

  void add_edge(unsigned vertexA, unsigned vertexB)
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

  vector<vector<unsigned>> bfs(unsigned vertex, string filename)
  {
    // Output vector
    vector<vector<unsigned>> output;
    // Vector of visited vertices
    vector<bool> visited = vector<bool>(this->n_vertices);
    // Queue for BFS
    queue<unsigned> q;
    // Vector of parents
    vector<unsigned> parent = vector<unsigned>(this->n_vertices);
    // Vector of levels
    vector<unsigned> level = vector<unsigned>(this->n_vertices);
    // Add initial vertex to queue, set its parent to itself and level to zero
    q.push(vertex - 1);
    parent[vertex - 1] = vertex - 1;
    level[vertex - 1] = 0;
    visited[vertex - 1] = true;
    // While queue is not empty
    while (q.size() > 0)
    {
      // Dequeue v from queue
      unsigned v = q.front();
      q.pop();
      // For each adjacent vertex of v
      for (unsigned i = 0; i < this->adjacencyMatrix[v].size(); i++)
        // If the adjacent vertex is not visited
        if ((this->adjacencyMatrix[v][i]) && (!visited[i]))
        {
          // Push adjacent vertex into queue
          q.push(i);
          // Mark adjacent vertex as visited
          visited[i] = true;
          // Set parent to v
          parent[i] = v;
          // Set level to parent level + 1
          level[i] = level[v] + 1;
        }
    }
    // Write results to file (if requested)
    if (!(filename == ""))
    {
      ofstream file(filename);
      for (unsigned int i = 0; i < this->n_vertices; i++)
        file << parent[i] + 1 << " " << level[i] << endl;
      file.close();
    }
    // Return output
    output.push_back(parent);
    output.push_back(level);
    return output;
  }

  unsigned get_distance(unsigned vertexA, unsigned vertexB)
  {
    vector<vector<unsigned>> bfsOutput = this->bfs(vertexA, "");
    return bfsOutput[1][vertexB - 1];
  }

  unsigned get_diameter()
  {
    unsigned diameter = 0;
    for (unsigned i = 0; i < this->n_vertices; i++)
    {
      vector<vector<unsigned>> bfsOutput = this->bfs(i + 1, "");
      for (unsigned j = 0; j < this->n_vertices; j++)
        if (bfsOutput[1][j] > diameter)
          diameter = bfsOutput[1][j];
    }
    return diameter;
  }

protected:
  vector<vector<bool>> adjacencyMatrix;
};
