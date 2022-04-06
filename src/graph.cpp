#include "graph.hpp"

template <class T>
Graph<T>::Graph(std::vector<T*> nodeData, std::vector<std::vector<uint>> edges) {

  // Check we have same ammount of nodes and edge entries
  if (nodeData.size() != edges.size()) {
    std::cout << "The size of the adjacency list must coincide with the ammount of nodes\n";
    exit(-1);
  }

  n = nodeData.size();

  nodes.resize(n);
  adjacency_list.resize(n);

  for (uint i = 0; i < n; ++i) {
    nodes[i] = new Node(nodeData[i]);
  }

  for (uint i = 0; i < edges.size(); ++i) {
    adjacency_list[i].resize(edges[i].size());
    for (uint j = 0; j < edges[i].size(); ++j) {
      adjacency_list[i][j] = new Edge(edges[i][j]);
    }
  }
}

template <class T>
Route* Graph<T>::dijkstra(uint from, uint to) {
  arrivalTime.resize(n);
  prevnode.resize(n);
  prevedge.resize(n);
  // Initialize arrival times to -1
  fill(arrivalTime.begin(), arrivalTime.end(), -1);

  // Will represent a pair of { c, n }, meaning we have a path of cost "c" to node "n"
  typedef std::pair<double, uint> state;
  // Create a minimum heap
  std::priority_queue<state, std::vector<state>, std::greater<state>> pq;

  // From node setup
  arrivalTime[from] = 0;
  pq.push({ 0, from });
  prevnode[0] = -1;
  prevedge[0] = -1;

  while (!pq.empty()) {
    // Get the best state so far
    double currentArrivalTime = pq.top().first;
    uint current = pq.top().second;
    T* currentNode = nodes[current]->node;
    pq.pop();

    // If this isn't the real best path to the node, skip it
    if (!double_equal(currentArrivalTime, arrivalTime[current])) {
      continue;
    }

    // Traverse all the edges
    for (uint edge_index = 0; edge_index < adjacency_list[current].size(); ++edge_index) {
      Edge* edge = adjacency_list[current][edge_index];

      uint neighbour = edge->to;

      T* nextNode = nodes[neighbour]->node;

      if (!currentNode->canGoTo(nextNode, currentArrivalTime)) {
        continue;
      }

      double nextArrivalTime = currentNode->edgeCost(nextNode);

      if (double_equal(arrivalTime[neighbour], -1) || double_less(nextArrivalTime, arrivalTime[neighbour])) {
        // If this contact was not reached yet, or we found a better path, update accordingly
        arrivalTime[neighbour] = nextArrivalTime;
        prevnode[neighbour] = current;
        prevedge[neighbour] = edge_index;
        pq.push({ nextArrivalTime, neighbour });
      }
    }
  }

  // If we did not find a path
  if (double_equal(arrivalTime[to], -1)) {
    return new Route({}, -1);
  }

  // Reconstruct the route backwards
  std::vector<T*> route;
  uint current = to;
  while (current != -1) {
    route.push_back(nodes[current]->node);
    current = prevnode[current];
  }
  // Reverse the route
  reverse(route.begin(), route.end());

  return new Route(route, arrivalTime[to]);
}

template <class T>
std::vector<Route*> Graph<T>::yen(uint from, uint to, uint ammount) {
  return { new Route({}, -1) };
}