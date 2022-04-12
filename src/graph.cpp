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
  edge_identifier.resize(n);

  for (uint i = 0; i < n; ++i) {
    nodes[i] = new GraphNode(nodeData[i]);
    node_identifier[nodeData[i]] = i;
  }


  for (uint i = 0; i < edges.size(); ++i) {
    adjacency_list[i].resize(edges[i].size());
    for (uint j = 0; j < edges[i].size(); ++j) {
      adjacency_list[i][j] = new Edge(edges[i][j]);
      edge_identifier[i][edges[i][j]] = j;
    }
  }
}

template <class T>
RouteT<T> Graph<T>::dijkstra(uint from, uint to) {
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
  prevnode[from] = -1;
  prevedge[from] = -1;

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

      // If the edge is supressed, don't go there
      if (edge->supressed) continue;

      uint neighbour = edge->to;

      T* nextNode = nodes[neighbour]->node;

      // If there is no way to go the next node, or it's supressed, continue
      if (!currentNode->canGoTo(nextNode, currentArrivalTime) || nodes[neighbour]->supressed) {
        continue;
      }

      double nextArrivalTime = currentNode->edgeCost(nextNode, currentArrivalTime);

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
    return new Route<T>({}, -1);
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
std::vector<RouteT<T>> Graph<T>::yen(uint from, uint to, uint ammount, std::vector<uint> to_supress) {
  std::vector<RouteT<T>> routes;

  RouteT<T> bestRoute = dijkstra(from, to);


  if (!bestRoute->isValid()) {
    // There are no routes in this case
    return routes;
  }

  // Supress the input nodes
  for (uint node_to_supress : to_supress) {
    nodes[node_to_supress]->supressed = true;
  }

  // Push the first route to the list
  routes.push_back(bestRoute);

  // Will represent a pair of {c, r}, meaning we have a route "r" with cost "c"
  typedef std::pair<double, RouteT<T>> state;

  // Create a minimum heap for the routes
  std::priority_queue<state, std::vector<state>, std::greater<state>> pq;

  for (uint k = 1; k < ammount; ++k) {
    // Get the previous best route
    std::vector<T*> lastRoute = routes[k - 1]->getRoute();

    RouteT<T> rootRoute = new Route<T>();

    // Initialize the matching routes
    std::vector<uint> matching_routes, next_matching_routes;
    for (uint i = 0; i < k; ++i) {
      matching_routes.push_back(i);
    }


    for (uint i = 0; i + 2 < lastRoute.size(); ++i) {
      std::vector<std::pair<uint, uint>> deleted_edges;

      T* spurNode = lastRoute[i];
      uint spurIdentifier = node_identifier[spurNode];

      for (RouteT<T> route : routes) {
        // Recalculate the matching routes based on the last vertex
        for (uint matching_route : matching_routes) {
          if (routes[matching_route]->getNode(i) == spurNode) {
            // We need to delete the edge with the next node
            uint next_node_id = node_identifier[routes[matching_route]->getNode(i + 1)];
            uint edge_to_delete = edge_identifier[spurIdentifier][next_node_id];
            adjacency_list[spurIdentifier][edge_to_delete]->supressed = true;

            // We later need to restore it
            deleted_edges.push_back(std::make_pair(spurIdentifier, edge_to_delete));

            // This route is still matching
            next_matching_routes.push_back(matching_route);
          }
        }
        swap(matching_routes, next_matching_routes);
        next_matching_routes.clear();
      }



      // Calculate the spur path from the spur node to the sink
      RouteT<T> spurPath = dijkstra(spurIdentifier, to);

      if (spurPath->isValid()) {
        RouteT<T> totalPath = new Route<T>();
        totalPath->merge(rootRoute)->merge(spurPath);
        pq.push({ totalPath->getRouteCost(), totalPath });
      }

      // Now the spur node is fixated
      rootRoute->addNode(spurNode);

      // Supress it for next time
      nodes[spurIdentifier]->supressed = true;

      // Restore the edges
      for (std::pair<uint, uint> edge : deleted_edges) {
        adjacency_list[edge.first][edge.second]->supressed = false;
      }
    }

    // Restore the nodes
    for (uint i = 0; i + 2 < lastRoute.size(); ++i) {
      uint node_id = node_identifier[rootRoute->getNode(i)];
      nodes[node_id]->supressed = false;
    }
    if (pq.empty()) {
      // No other paths available
      break;
    }
    routes.push_back(pq.top().second);
    pq.pop();
  }

  // Unsupress the input nodes
  for (uint node_to_supress : to_supress) {
    nodes[node_to_supress]->supressed = false;
  }

  return routes;
}

template class Graph<Contact>;