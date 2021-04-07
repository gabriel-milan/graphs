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
  virtual void writeInfoTo(const char *filename){};
  virtual void add_edge(unsigned vertexA, unsigned vertexB){};
  virtual vector<vector<unsigned>> bfs(unsigned vertex, const char *filename) { return vector<vector<unsigned>>(0); };
  virtual void dfs(unsigned vertex, const char *filename){};
  virtual unsigned getDistance(unsigned start, unsigned end) { return 0; };
  virtual unsigned getDiameter() { return 0; };
  virtual void getComponents(){};

protected:
  unsigned n_edges;
  unsigned n_vertices;
};