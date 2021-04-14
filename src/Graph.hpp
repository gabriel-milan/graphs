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
  virtual void info_to_file(const char *filename){};
  virtual void add_edge(int vertexA, int vertexB){};
  virtual vector<vector<int>> bfs(int vertex, const char *filename) { return vector<vector<int>>(0); };
  virtual vector<vector<int>> dfs(int vertex, const char *filename) { return vector<vector<int>>(0); };
  virtual vector<double> get_degree_info() { return vector<double>(0); };
  virtual unsigned get_distance(int start, int end) { return 0; };
  virtual unsigned get_diameter() { return 0; };
  virtual unsigned count_components() { return 0; };
  virtual vector<vector<int>> get_components() { return vector<vector<int>>(0); };
  virtual unsigned get_n_edges() { return this->n_edges; };
  virtual unsigned get_n_vertices() { return this->n_vertices; };

protected:
  unsigned n_edges;
  unsigned n_vertices;
};
