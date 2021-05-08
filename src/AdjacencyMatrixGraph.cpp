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

  vector<double> get_degree_info()
  {
    // Output vector
    vector<double> output;
    // Store degree information
    vector<int> degrees;
    // Storing stuff
    double maxDegree = 0;
    double minDegree = numeric_limits<double>::max();
    double averageDegree = 0;
    double medianDegree = 0;
    // For each vertex, get degree information
    for (unsigned i = 0; i < this->n_vertices; i++)
    {
      // Get degree
      double degree = (double)count(this->adjacencyMatrix[i].begin(), this->adjacencyMatrix[i].end(), true);
      // Check if it is maximum
      if (degree > maxDegree)
        maxDegree = degree;
      // Check if it is mininum
      if (degree < minDegree)
        minDegree = degree;
      // Sum for average degree
      averageDegree += degree;
      // Store on vector for median
      degrees.push_back((int)degree);
    }
    // Dividing average by n_vertices
    averageDegree /= (double)this->n_vertices;
    // Sort vector for computing median
    sort(degrees.begin(), degrees.end());
    // If n_vertices is odd
    if (this->n_vertices % 2 == 1)
      medianDegree = degrees[(this->n_vertices - 1) / 2];
    // n_vertices is even
    else
      medianDegree = (degrees[this->n_vertices / 2] + degrees[(this->n_vertices / 2) - 1]) / 2;
    output.push_back(minDegree);
    output.push_back(maxDegree);
    output.push_back(averageDegree);
    output.push_back(medianDegree);
    return output;
  }

  unsigned get_distance(int vertexA, int vertexB)
  {
    vector<vector<int>> bfsOutput = this->bfs(vertexA, "");
    return bfsOutput[1][vertexB - 1];
  }

  void test(mutex *l);

  unsigned get_diameter()
  {
    unsigned diameter = 0;
    vector<unsigned> diameters;
    mutex *lock = new mutex();
    parallel_for<unsigned>(
        this->n_vertices, lock, &diameters, [this](int start, int end, mutex *lock, vector<unsigned> *results) {
          this->process_chunk_for_diameter(start, end, lock, results);
        },
        true);
    for (size_t i = 0; i < diameters.size(); i++)
      if (diameters[i] > diameter)
        diameter = diameters[i];
    return diameter;
  }

  unsigned
  count_components()
  {
    unsigned components = 0;
    vector<bool> visited = vector<bool>(n_vertices);
    unsigned i = 0;
    bool nextDefined = true;
    while (i < this->n_vertices)
    {
      components++;
      nextDefined = false;
      visited[i] = true;
      vector<vector<int>> results = this->bfs(i + 1, "");
      for (unsigned j = i; j < this->n_vertices; j++)
        if (results[2][j] == 1)
          visited[j] = true;
        else if ((!nextDefined) && (!visited[j]))
        {
          nextDefined = true;
          i = j;
        }
      if (!nextDefined)
        i = (int)this->n_vertices;
    }
    return components;
  }

  vector<vector<int>> get_components()
  {
    vector<vector<int>> components;
    vector<bool> visited = vector<bool>(n_vertices);
    unsigned i = 0;
    bool nextDefined = true;
    while (i < this->n_vertices)
    {
      vector<int> component;
      nextDefined = false;
      visited[i] = true;
      vector<vector<int>> results = this->bfs(i + 1, "");
      for (unsigned j = i; j < this->n_vertices; j++)
        if (results[2][j] == 1)
        {
          visited[j] = true;
          component.push_back(j);
        }
        else if ((!nextDefined) && (!visited[j]))
        {
          nextDefined = true;
          i = j;
        }
      if (!nextDefined)
        i = (int)this->n_vertices;
      components.push_back(component);
    }
    return components;
  }

protected:
  vector<vector<bool>> adjacencyMatrix;

  void process_chunk_for_diameter(int start, int end, mutex *lock, vector<unsigned> *diameters)
  {
    unsigned d = 0;
    for (unsigned i = start; i < end; i++)
    {
      vector<vector<int>> bfsOutput = this->bfs(i + 1, "");
      for (unsigned j = 0; j < this->get_n_vertices(); j++)
        if ((bfsOutput[1][j] > (int)d) && (bfsOutput[1][j] != numeric_limits<int>::max()))
          d = bfsOutput[1][j];
    }
    lock->lock();
    diameters->push_back(d);
    lock->unlock();
  }
};
