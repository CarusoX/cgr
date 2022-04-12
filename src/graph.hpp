#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm>
#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include "contact.hpp"
#include "graph_node.hpp"
#include "route.hpp"

struct Edge {
  uint to;
  bool supressed;
  Edge(uint _to) : to(_to), supressed(0) {}
};

template <class T>
class Graph {

private:
  uint n; // the ammount of nodes in the graph

  std::vector<GraphNodeT<T>> nodes; // the nodes of the graph

  std::vector<std::vector<Edge*>> adjacency_list; // graph represented by an adjacency list

  std::unordered_map<T*, uint> node_identifier; // node -> identifier
  std::vector<std::unordered_map<uint, uint>> edge_identifier; // edge -> identifier

  // ---------------------------------------------------------------------------
  // Dijkstra related stuff
  // ---------------------------------------------------------------------------
  std::vector<double> arrivalTime;
  std::vector<uint> prevnode;
  std::vector<uint> prevedge;


  // ---------------------------------------------------------------------------
  // Yen related stuff
  // ---------------------------------------------------------------------------=

public:
  Graph(std::vector<T*> nodeData, std::vector<std::vector<uint>> edges);

  RouteT<T> dijkstra(uint from, uint to);

  std::vector<RouteT<T>> yen(uint from, uint to, uint ammount, std::vector<uint> to_supress = {});

  void debug() {
    std::cout << "Printing the graph" << std::endl;

    for (uint i = 0; i < n; ++i) {
      std::cout << "Edges of node " << i << ":" << std::endl;
      for (uint j = 0; j < adjacency_list[i].size(); ++j) {
        std::cout << adjacency_list[i][j] << std::endl;
      }
    }
  }
};

template <class T>
using GraphT = Graph<T>*;

#endif