#pragma once

#include "MinHeap.hpp"

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
  virtual vector<tuple<int, float>> get_neighbors(int vertex) { return vector<tuple<int, float>>(0); };

  unsigned get_n_edges() { return this->n_edges; };

  unsigned get_n_vertices() { return this->n_vertices; };

  void info_to_file(string filename)
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

  void export_to_file(string filename)
  {
    ofstream file(filename);
    // Write number of vertices
    file << this->n_vertices << endl;
    // Write all edges only once
    for (unsigned i = 0; i < this->n_vertices; i++)
    {
      vector<tuple<int, float>> neighbors = this->get_neighbors(i);
      for (auto it = neighbors.begin(); it != neighbors.end(); ++it)
        if (get<0>(*it) < i)
          if (this->is_weighted)
            file << i + 1 << " " << get<0>(*it) + 1 << " " << get<1>(*it) << endl;
          else
            file << i + 1 << " " << get<0>(*it) + 1 << endl;
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
      vector<tuple<int, float>> neighbors = this->get_neighbors(v);
      for (auto it = neighbors.begin(); it != neighbors.end(); ++it)
      {
        int it_index = get<0>(*it);
        // If the adjacent vertex is not visited
        if (!visited[it_index])
        {
          // Push adjacent vertex into queue
          q.push(it_index);
          // Mark adjacent vertex as visited
          visited[it_index] = true;
          // Set parent to v
          parent[it_index] = v;
          // Set level to parent level + 1
          level[it_index] = level[v] + 1;
        }
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
      vector<tuple<int, float>> neighbors = this->get_neighbors(u);
      // If this vertex has no parent
      if (parent[u] == -1)
      {
        // A possible parent for it is the top of the parent stack
        int possibleParent = parentStack.top();
        // While this possible parent has no edge with vertex "u"
        while (!contain_tuples_vec(neighbors, possibleParent))
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
        for (auto it = neighbors.rbegin(); it != neighbors.rend(); ++it)
          // Push adjacent vertex into stack
          s.push(get<0>(*it));
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

  vector<vector<float>> dijkstra(int vertex, string filename)
  {
    // Checking if graph has negative weights
    if (this->has_negative_weight)
    {
      cout << "[ERROR] This graph has negative weights, Dijkstra algorithm won't work!" << endl;
      return vector<vector<float>>(0);
    }
    // Fixing 1-indexed to 0-indexed
    vertex--;
    // Output vector
    vector<vector<float>> output;
    // Vector of visited vertices
    vector<bool> visited = vector<bool>(this->n_vertices);
    // Vector of parents
    vector<float> parent = vector<float>(this->n_vertices, -1); // Default parent is -1
    // Vector of distances
    vector<float> distance = vector<float>(this->n_vertices, numeric_limits<int>::max()); // Default level is max of integer
    // Vector of visited vertices using ints (for output)
    vector<float> visitedOutput = vector<float>(this->n_vertices);
    // Min binary heap
    MinHeap minHeap = MinHeap(this->n_vertices);
    // Initialize min heap
    for (unsigned i = 0; i < this->n_vertices; i++)
    {
      minHeap.harr[i] = newMinHeapNode(i, distance[i]);
      minHeap.pos[i] = i;
    }
    minHeap.heap_size = (int)this->n_vertices;
    // Set initial vertex distance to 0 and its parent to itself
    distance[vertex] = 0;
    parent[vertex] = (float)vertex;
    minHeap.decreaseKey(vertex, distance[vertex]);
    // While the heap is not empty
    while (!minHeap.isEmpty())
    {
      // Get vertex "u" of minimum distance
      // and remove it from the heap
      MinHeapNode *minVertex = minHeap.extractMin();
      int u = minVertex->v;
      // Mark as visited
      visited[u] = true;
      // For each adjacent vertex "v" of "u"
      vector<tuple<int, float>> neighbors = this->get_neighbors(u);
      for (auto it = neighbors.begin(); it != neighbors.end(); ++it)
      {
        int v = get<0>(*it);
        float weight = get<1>(*it);
        if (minHeap.isInMinHeap(v) && distance[v] > weight + distance[u])
        {
          parent[v] = (float)u;
          distance[v] = distance[u] + weight;
          minHeap.decreaseKey(v, distance[v]);
        }
      }
    }
    // Write results to file (if requested)
    if (!(filename == ""))
    {
      ofstream file(filename);
      for (unsigned int i = 0; i < this->n_vertices; i++)
        file << parent[i] + 1 << " " << distance[i] << endl;
      file.close();
    }
    // Generate visited output
    for (unsigned int i = 0; i < this->n_vertices; i++)
      if (visited[i])
        visitedOutput[i] = 1;
    // Return output
    output.push_back(parent);
    output.push_back(distance);
    output.push_back(visitedOutput);
    return output;
  }

  vector<vector<float>> prim(int vertex, string filename)
  {
    // Fixing 1-indexed to 0-indexed
    vertex--;
    // Output vector
    vector<vector<float>> output;
    // Vector of visited vertices
    vector<bool> visited = vector<bool>(this->n_vertices);
    // Vector of parents
    vector<float> parent = vector<float>(this->n_vertices, -1); // Default parent is -1
    // Vector of costs
    vector<float> cost = vector<float>(this->n_vertices, numeric_limits<int>::max()); // Default level is max of integer
    // Vector of visited vertices using ints (for output)
    vector<float> visitedOutput = vector<float>(this->n_vertices);
    // Min binary heap
    MinHeap minHeap = MinHeap(this->n_vertices);
    // Initialize min heap
    for (unsigned i = 0; i < this->n_vertices; i++)
    {
      minHeap.harr[i] = newMinHeapNode(i, cost[i]);
      minHeap.pos[i] = i;
    }
    minHeap.heap_size = (int)this->n_vertices;
    // Set initial vertex cost to 0 and its parent to itself
    cost[vertex] = 0;
    parent[vertex] = (float)vertex;
    minHeap.decreaseKey(vertex, cost[vertex]);
    // While the heap is not empty
    while (!minHeap.isEmpty())
    {
      // Get vertex "u" of minimum cost
      // and remove it from the heap
      MinHeapNode *minVertex = minHeap.extractMin();
      int u = minVertex->v;
      // Mark as visited
      visited[u] = true;
      // For each adjacent vertex "v" of "u"
      vector<tuple<int, float>> neighbors = this->get_neighbors(u);
      for (auto it = neighbors.begin(); it != neighbors.end(); ++it)
      {
        int v = get<0>(*it);
        float weight = get<1>(*it);
        if (minHeap.isInMinHeap(v) && cost[v] > weight)
        {
          parent[v] = (float)u;
          cost[v] = weight;
          minHeap.decreaseKey(v, cost[v]);
        }
      }
    }
    // Write results to file (if requested)
    if (!(filename == ""))
    {
      ofstream file(filename);
      file << this->n_vertices << endl;
      for (unsigned int i = 0; i < this->n_vertices; i++)
      {
        if (cost[i] != 0)
          file << i + 1 << " " << parent[i] + 1 << " " << cost[i] << endl;
      }
      file.close();
    }
    // Generate visited output
    for (unsigned int i = 0; i < this->n_vertices; i++)
      if (visited[i])
        visitedOutput[i] = 1;
    // Return output
    output.push_back(parent);
    output.push_back(cost);
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
      double degree = (double)this->get_neighbors(i).size();
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

  vector<float> build_path_from_parents(float vertexA, float vertexB, vector<float> parents)
  {
    vector<float> output;
    vertexA--;
    vertexB--;
    output.push_back(vertexB);
    while (parents[vertexB] != vertexA)
    {
      output.push_back(parents[vertexB]);
      vertexB = parents[vertexB];
    }
    output.push_back(vertexA);
    reverse(output.begin(), output.end());
    return output;
  }

  vector<vector<float>> get_all_paths(int vertexA)
  {
    vector<vector<float>> paths;
    if (this->is_weighted)
      if (!this->has_negative_weight)
      {
        vector<vector<float>> output = this->dijkstra(vertexA, "");
        for (int vertexB = 1; vertexB <= this->n_vertices; vertexB++)
        {
          if ((output[1][vertexB - 1] == numeric_limits<int>::max()) || (vertexA == vertexB))
            paths.push_back(vector<float>(0));
          else
            paths.push_back(build_path_from_parents((float)vertexA, (float)vertexB, output[0]));
        }
        return paths;
      }
      else
      {
        cout << "[ERROR] Can't compute paths for a graph with negative weights." << endl;
        return vector<vector<float>>(0);
      }
    else
    {
      vector<vector<int>> bfsOutput = this->bfs(vertexA, "");
      vector<float> output;
      for (auto it = bfsOutput[0].begin(); it != bfsOutput[0].end(); ++it)
        output.push_back((float)*it);
      for (int vertexB = 1; vertexB <= this->n_vertices; vertexB++)
      {
        if ((bfsOutput[1][vertexB - 1] == numeric_limits<int>::max()) || (vertexA == vertexB))
          paths.push_back(vector<float>(0));
        else
          paths.push_back(build_path_from_parents((float)vertexA, (float)vertexB, output));
      }
      return paths;
    }
  }

  vector<float> get_path(int vertexA, int vertexB)
  {
    if (this->is_weighted)
      if (!this->has_negative_weight)
      {
        vector<vector<float>> output = this->dijkstra(vertexA, "");
        if (output[1][vertexB - 1] == numeric_limits<int>::max())
          return vector<float>(0);
        return build_path_from_parents((float)vertexA, (float)vertexB, output[0]);
      }
      else
      {
        cout << "[ERROR] Can't compute paths for a graph with negative weights." << endl;
        return vector<float>(0);
      }
    else
    {
      vector<vector<int>> bfsOutput = this->bfs(vertexA, "");
      vector<float> output;
      for (auto it = bfsOutput[0].begin(); it != bfsOutput[0].end(); ++it)
        output.push_back((float)*it);
      if (bfsOutput[1][vertexB - 1] == numeric_limits<int>::max())
        return vector<float>(0);
      return build_path_from_parents((float)vertexA, (float)vertexB, output);
    }
  }

  vector<float> get_all_distances(int vertexA)
  {
    if (this->is_weighted)
      if (!this->has_negative_weight)
      {
        vector<vector<float>> output = this->dijkstra(vertexA, "");
        return output[1];
      }
      else
      {
        cout << "[ERROR] Can't compute distances for a graph with negative weights." << endl;
        return vector<float>(this->n_vertices, numeric_limits<int>::max());
      }
    else
    {
      vector<vector<int>> bfsOutput = this->bfs(vertexA, "");
      vector<float> output;
      for (auto it = bfsOutput[1].begin(); it != bfsOutput[1].end(); ++it)
        output.push_back((float)*it);
      return output;
    }
  }

  unsigned get_distance(int vertexA, int vertexB)
  {
    return this->get_all_distances(vertexA)[vertexB - 1];
  }

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

  unsigned count_components()
  {
    return this->get_components().size();
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
  unsigned n_edges;
  unsigned n_vertices;
  bool is_weighted = false;
  bool has_negative_weight = false;
  void process_chunk_for_diameter(int start, int end, mutex *lock, vector<unsigned> *diameters)
  {
    unsigned d = 0;
    for (int i = start; i < end; i++)
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
