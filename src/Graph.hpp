#pragma once

using namespace std;

/*
 * Interface for representing
 * graphs
 */
class Graph
{
public:
  Graph(){};
  virtual void add_edge(int vertexA, int vertexB){};
  virtual vector<double> get_degree_info() { return vector<double>(0); };
  virtual unsigned get_distance(int start, int end) { return 0; };
  virtual unsigned get_diameter() { return 0; };
  virtual unsigned count_components() { return 0; };
  virtual vector<vector<int>> get_components() { return vector<vector<int>>(0); };
  virtual unsigned get_n_edges() { return this->n_edges; };
  virtual unsigned get_n_vertices() { return this->n_vertices; };
  virtual vector<int> get_neighbors(int vertex){};

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

  vector<vector<int>> bfs(int vertex, const char *filename)
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
      vector<int> neighbors = this->get_neighbors(v);
      for (auto it = neighbors.begin(); it != neighbors.end(); ++it)
        // If the adjacent vertex is not visited
        if (!visited[*it])
        {
          // Push adjacent vertex into queue
          q.push(*it);
          // Mark adjacent vertex as visited
          visited[*it] = true;
          // Set parent to v
          parent[*it] = v;
          // Set level to parent level + 1
          level[*it] = level[v] + 1;
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

  vector<vector<int>> dfs(int vertex, const char *filename)
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
      vector<int> neighbors = this->get_neighbors(u);
      // If this vertex has no parent
      if (parent[u] == -1)
      {
        // A possible parent for it is the top of the parent stack
        int possibleParent = parentStack.top();
        // While this possible parent has no edge with vertex "u"
        while (!Contains(neighbors, possibleParent))
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
        // For each adjacent vertex of u
        for (vector<int>::reverse_iterator it = neighbors.rbegin(); it != neighbors.rend(); ++it)
          // Push adjacent vertex into stack
          s.push(*it);
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

protected:
  unsigned n_edges;
  unsigned n_vertices;
};
