#pragma once

#include <bits/stdc++.h>
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
 * a adjacency vectors
 */
class AdjacencyVectorsGraph : public Graph
{
public:
  AdjacencyVectorsGraph(const char *filename)
  {
    // str will be used to store file lines
    string str;
    ifstream graphFile(filename);
    // Read first line in order to get number of vertices
    getline(graphFile, str);
    unsigned vertices = (unsigned)stoi(str);
    this->n_vertices = vertices;
    // Generate adjacency vectors
    this->adjacencyVectors = vector<vector<unsigned>>(vertices);
    // Iterate over file getting edges and adding them
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
    this->n_edges++;
    this->adjacencyVectors.at(vertexA - 1).push_back(vertexB);
    sort(this->adjacencyVectors.at(vertexA - 1).begin(), this->adjacencyVectors.at(vertexA - 1).end());
    this->adjacencyVectors.at(vertexB - 1).push_back(vertexA);
    sort(this->adjacencyVectors.at(vertexB - 1).begin(), this->adjacencyVectors.at(vertexB - 1).end());
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
      for (unsigned i = 0; i < this->adjacencyVectors[v].size(); i++)
        // If the adjacent vertex is not visited
        if (!visited[this->adjacencyVectors[v][i] - 1])
        {
          // Push adjacent vertex into queue
          q.push(this->adjacencyVectors[v][i] - 1);
          // Mark adjacent vertex as visited
          visited[this->adjacencyVectors[v][i] - 1] = true;
          // Set parent to v
          parent[this->adjacencyVectors[v][i] - 1] = v;
          // Set level to parent level + 1
          level[this->adjacencyVectors[v][i] - 1] = level[v] + 1;
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
  vector<vector<unsigned>> adjacencyVectors;
};