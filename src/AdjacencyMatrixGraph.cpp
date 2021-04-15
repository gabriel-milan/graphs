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

  void info_to_file(const char *filename)
  {
    vector<double> degreeInfo = this->get_degree_info();
    ofstream file(filename);
    file << "n_vertices=" << this->n_vertices << endl;
    file << "n_edges=" << this->n_edges << endl;
    file << "degree_min=" << degreeInfo[0] << endl;
    file << "degree_max=" << degreeInfo[1] << endl;
    file << "degree_avg=" << degreeInfo[2] << endl;
    file << "degree_median=" << degreeInfo[3] << endl;
    file << "components=" << this->count_components() << endl;
    file.close();
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

  vector<vector<int>> bfs(int vertex, string filename)
  {
    // Output vector
    vector<vector<int>> output;
    // Vector of visited vertices
    vector<bool> visited = vector<bool>(this->n_vertices);
    // Queue for BFS
    queue<int> q;
    // Vector of parents
    vector<int> parent = vector<int>(this->n_vertices, -1); // Default parent is -1
    // Vector of levels
    vector<int> level = vector<int>(this->n_vertices, numeric_limits<int>::max()); // Default level is max of integer
    // Vector of visited vertices using ints (for output)
    vector<int> visitedOutput = vector<int>(this->n_vertices);
    // Add initial vertex to queue, set its parent to itself and level to zero
    q.push(vertex - 1);
    parent[vertex - 1] = vertex - 1;
    level[vertex - 1] = 0;
    visited[vertex - 1] = true;
    // While queue is not empty
    while (q.size() > 0)
    {
      // Dequeue v from queue
      int v = q.front();
      q.pop();
      // For each adjacent vertex of v
      for (size_t i = 0; i < this->adjacencyMatrix[v].size(); i++)
        // If the adjacent vertex is not visited
        if ((this->adjacencyMatrix[v][i]) && (!visited[i]))
        {
          // Push adjacent vertex into queue
          q.push((int)i);
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
    // Generate visited output
    for (unsigned int i = 0; i < this->n_vertices; i++)
      if (visited[i])
        visitedOutput[i] = 1;
    // Return output
    output.push_back(parent);
    output.push_back(level);
    output.push_back(visitedOutput);
    return output;
  }

  vector<vector<int>> dfs(int vertex, string filename)
  {
    // Output vector
    vector<vector<int>> output;
    // Vector of visited vertices
    vector<bool> visited = vector<bool>(this->n_vertices);
    // Stack for DFS
    stack<int> s;
    // Stack for keeping track of parenting
    stack<int> parentStack;
    // Vector of parents
    vector<int> parent = vector<int>(this->n_vertices, -1); // Default parent is -1
    // Vector of levels
    vector<int> level = vector<int>(this->n_vertices, numeric_limits<int>::max()); // Default level is max of integer
    // Vector of visited vertices using ints (for output)
    vector<int> visitedOutput = vector<int>(this->n_vertices);
    // Add initial vertex to queue, set its parent to itself and level to zero
    s.push(vertex - 1);
    parent[vertex - 1] = vertex - 1;
    level[vertex - 1] = 0;
    // While queue is not empty
    while (!s.empty())
    {
      // Pop vertex from stack
      int u = s.top();
      s.pop();
      // If this vertex has no parent
      if (parent[u] == -1)
      {
        // A possible parent for it is the top of the parent stack
        int possibleParent = parentStack.top();
        // While this possible parent has no edge with vertex "u"
        while (!this->adjacencyMatrix[u][possibleParent])
        {
          // Pop parent from stack and get another
          parentStack.pop();
          possibleParent = parentStack.top();
        }
        // Set parent and level to parent's + 1
        parent[u] = possibleParent;
        level[u] = level[possibleParent] + 1;
      }
      // If u is not marked
      if (!visited[u])
      {
        // Add "u" to parent stack
        parentStack.push(u);
        // Mark u as visited
        visited[u] = true;
        // For each vertex of graph
        for (int i = (int)(this->n_vertices) - 1; i >= 0; i--)
          // If vertex is adjacent to "u"
          if (this->adjacencyMatrix[u][i])
            // Push vertex into stack
            s.push(i);
      }
    }
    // Write results to file (if requested)
    if (!(filename == ""))
    {
      ofstream file(filename);
      for (unsigned i = 0; i < this->n_vertices; i++)
        file << parent[i] + 1 << " " << level[i] << endl;
      file.close();
    }
    // Generate visited output
    for (unsigned int i = 0; i < this->n_vertices; i++)
      if (visited[i])
        visitedOutput[i] = 1;
    // Return output
    output.push_back(parent);
    output.push_back(level);
    output.push_back(visitedOutput);
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
    parallel_for<unsigned>(this->n_vertices, lock, diameters, [this](int start, int end, mutex *lock, vector<unsigned> results) {
      this->process_chunk_for_diameter(start, end, lock, results);
    });
    for (unsigned i = 0; i < this->n_vertices; i++)
    {
      vector<vector<int>> bfsOutput = this->bfs(i + 1, "");
      for (unsigned j = 0; j < this->n_vertices; j++)
        if ((bfsOutput[1][j] > (int)diameter) && (bfsOutput[1][j] != numeric_limits<int>::max()))
          diameter = bfsOutput[1][j];
    }
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

  void process_chunk_for_diameter(int start, int end, mutex *lock, vector<unsigned> diameters)
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
    diameters.push_back(d);
    lock->unlock();
  }
};
