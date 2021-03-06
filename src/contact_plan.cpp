#include "contact_plan.hpp"

void ContactPlan::add_participant_to_dictionary(std::string participant) {
  if (!participant_to_identifier.count(participant)) {
    // We assign the participant a new identifier
    participant_to_identifier[participant] = participant_to_identifier.size();
  }
}

void ContactPlan::assert_participant_exists(std::string participant) {
  if (!participant_to_identifier.count(participant)) {
    std::cerr << participant << " is not a valid participant.\n";
    exit(-1);
  }
}

void ContactPlan::build_graph() {

  adjacency_list.resize(n);

  for (ContactT contact : contacts) {
    uint from = participant_to_identifier[contact->getFrom()];
    uint to = participant_to_identifier[contact->getTo()];
    adjacency_list[from].push_back(new Edge(to, contact));
  }
}

ContactPlan::ContactPlan(std::vector<ContactT> _contacts) {

  // We assign each participant a new identifier
  for (ContactT contact : _contacts) {
    add_participant_to_dictionary(contact->getFrom());
    add_participant_to_dictionary(contact->getTo());
  }

  // Insert the contacts
  contacts = _contacts;

  // Set the number of contacts
  n = contacts.size();

  // Build the graph
  build_graph();
}

RouteT<Contact> ContactPlan::path_to_route(path path) {
  // Return an invalid route if there is no path
  if (path.size() == 0) return new Route<Contact>({}, -1);

  RouteT<Contact> route = new Route<Contact>();

  for (uint i = 0; i + 1 < path.size(); ++i) {
    const auto& [node, edgeTaken] = path[i];
    ContactT contactToAdd = adjacency_list[node][edgeTaken]->contact;
    route->addNode(contactToAdd);
  }

  return route;
}

std::pair<path, double> ContactPlan::cgr_dijkstra(uint from, uint to, double start) {

  // Initialize arrival times to -1
  arrivalTime.resize(n);
  supressed.resize(n, false);
  prevnode.resize(n);
  prevedge.resize(n);

  fill(arrivalTime.begin(), arrivalTime.end(), -1);

  // Will represent a pair of { c, n }, meaning we have a path of cost "c" to node "n"
  typedef std::pair<double, uint> state;
  // Create a minimum heap
  std::priority_queue<state, std::vector<state>, std::greater<state>> pq;

  // From node setup
  arrivalTime[from] = start;
  pq.push({ start, from });
  prevnode[from] = -1;
  prevedge[from] = -1;

  while (!pq.empty()) {
    // Get the best state so far
    const auto [currentArrivalTime, current] = pq.top();

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
      ContactT neighbourContact = edge->contact;

      // If there is no way to go the next node, or it's supressed, continue
      if (!neighbourContact->canGoTo(currentArrivalTime) || supressed[neighbour]) {
        continue;
      }

      double nextArrivalTime = neighbourContact->edgeCost(currentArrivalTime);

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
    return {};
  }

  // Reconstruct the route backwards
  path route;
  uint current = to;
  uint current_edge = -1;
  while (current != uint(-1)) {
    route.push_back({ current, current_edge });
    current_edge = prevedge[current];
    current = prevnode[current];
  }
  // Reverse the route
  reverse(route.begin(), route.end());

  return { route, arrivalTime[to] };
}

RouteT<Contact> ContactPlan::dijkstra(std::string from, std::string to) {

  // Assert from and to exists
  assert_participant_exists(from);
  assert_participant_exists(to);

  uint from_index = participant_to_identifier[from];
  uint to_index = participant_to_identifier[to];

  return path_to_route(cgr_dijkstra(from_index, to_index).first);
}

std::vector<RouteT<Contact>> ContactPlan::cgr_yen(uint from, uint to, uint ammount) {
  std::vector<path> paths;

  path bestPath = cgr_dijkstra(from, to).first;


  if (bestPath.size() == 0) {
    // There are no routes in this case
    return {};
  }

  // Push the first route to the list
  paths.push_back(bestPath);

  // Will represent a pair of {{c, d}, p}, meaning we have a path "p" with cost "c" that deviated from parent at index "d"
  typedef std::pair<std::pair<double, uint>, path> state;

  uint deviation = 0;

  // Create a minimum heap for the paths
  std::priority_queue<state, std::vector<state>, std::function<bool(state, state)>> pq(path_with_cost_greater);

  for (uint k = 1; k < ammount; ++k) {
    // Get the previous best path
    path lastRoute = paths[k - 1];

    path rootPath = {};
    double rootPathCost = 0;

    // Initialize the matching routes
    std::vector<uint> matching_paths, next_matching_paths;
    for (uint i = 0; i < k; ++i) {
      matching_paths.push_back(i);
    }

    for (uint i = 0; i + 1 < lastRoute.size(); ++i) {
      std::vector<std::pair<uint, uint>> deleted_edges;

      const auto [spurNode, spurEdgeTaken] = lastRoute[i];
      // Recalculate the matching routes based on the last vertex
      for (uint matching_path : matching_paths) {
        const auto [matchingNode, matchingEdgeTaken] = paths[matching_path][i];
        if (matchingNode == spurNode) {
          // We need to delete the edge with the next node
          adjacency_list[spurNode][matchingEdgeTaken]->supressed = true;

          // We later need to restore it
          deleted_edges.push_back({ spurNode, matchingEdgeTaken });

          if (matchingEdgeTaken == spurEdgeTaken) {
            // This route is still matching
            next_matching_paths.push_back(matching_path);
          }
        }
      }
      swap(matching_paths, next_matching_paths);
      next_matching_paths.clear();

      std::pair<path, double> spur_dijkstra_run = { {}, -1 };

      if (deviation <= i) {
        // Calculate the spur path from the spur node to the sink
        spur_dijkstra_run = cgr_dijkstra(spurNode, to, rootPathCost);
      }

      path spurPath = spur_dijkstra_run.first;
      double spurPathCost = spur_dijkstra_run.second;

      if (spurPath.size() > 0) {
        // Build the total path
        path totalPath = {};
        totalPath.insert(totalPath.end(), rootPath.begin(), rootPath.end());
        totalPath.insert(totalPath.end(), spurPath.begin(), spurPath.end());
        // The path deviated at node i
        pq.push({ { spurPathCost, i }, totalPath });
      }

      // Now the spur node is added to the root path
      rootPath.push_back(lastRoute[i]);
      ContactT usedContact = adjacency_list[spurNode][spurEdgeTaken]->contact;
      rootPathCost = usedContact->edgeCost(rootPathCost);

      // Supress it for next time
      supressed[spurNode] = true;

      // Restore the edges
      for (const auto& [node, edgeTaken] : deleted_edges) {
        adjacency_list[node][edgeTaken]->supressed = false;
      }
    }

    // Restore the nodes
    for (uint i = 0; i + 1 < lastRoute.size(); ++i) {
      supressed[lastRoute[i].first] = false;
    }

    // If we already have enough paths, just toss them
    if (ammount <= pq.size() + paths.size()) {
      while (paths.size() != ammount) {
        paths.push_back(pq.top().second);
        pq.pop();
      }
      break;
    }


    if (pq.empty()) {
      // No other paths available
      break;
    }

    path nextPath = pq.top().second;
    deviation = pq.top().first.second;

    pq.pop();

    // Add the new path
    paths.push_back(nextPath);
  }

  std::vector<RouteT<Contact>> routes;
  for (path path : paths) {
    routes.push_back(path_to_route(path));
  }

  return routes;
}

std::vector<RouteT<Contact>> ContactPlan::yen(std::string from, std::string to, uint ammount) {

  // Assert from and to exists
  assert_participant_exists(from);
  assert_participant_exists(to);

  uint from_index = participant_to_identifier[from];
  uint to_index = participant_to_identifier[to];

  return cgr_yen(from_index, to_index, ammount);
}

void ContactPlan::cgr_dfs(uint where, uint to, std::vector<RouteT<Contact>>& routes, double currentTime) {

  if (where == to) {
    dfs_stack.push_back({ where, -1 });
    routes.push_back(path_to_route(dfs_stack));
    dfs_stack.pop_back();
    return;
  }

  dfs_visited[where] = 1;

  for (uint i = 0; i < adjacency_list[where].size(); ++i) {
    Edge* edge = adjacency_list[where][i];
    if (!dfs_visited[edge->to] && edge->contact->canGoTo(currentTime)) {
      dfs_stack.push_back({ where, i });
      cgr_dfs(edge->to, to, routes, edge->contact->edgeCost(currentTime));
      dfs_stack.pop_back();
    }
  }

  dfs_visited[where] = 0;
}

std::vector<RouteT<Contact>> ContactPlan::dfs(std::string from, std::string to) {
  // Assert from and to exists
  assert_participant_exists(from);
  assert_participant_exists(to);

  uint from_index = participant_to_identifier[from];
  uint to_index = participant_to_identifier[to];

  std::vector<RouteT<Contact>> routes;

  dfs_visited.resize(n);
  fill(dfs_visited.begin(), dfs_visited.end(), 0);

  cgr_dfs(from_index, to_index, routes);

  sort(routes.begin(), routes.end(), [](RouteT<Contact> routeA, RouteT<Contact> routeB)->bool {
    return (*routeA) < routeB;
    });

  return routes;
}

std::vector<RouteT<Contact>> ContactPlan::cgr_first_ended(uint from, uint to) {
  std::vector<RouteT<Contact>> routes;

  std::vector<std::pair<uint, uint>> deleted_edges;

  while (1) {
    std::pair<path, double> dijkstra_run = cgr_dijkstra(from, to);

    path path = dijkstra_run.first;

    if (path.size() == 0) {
      // No more routes
      break;
    }

    uint min_end = ~0;

    for (uint i = 0; i + 1 < path.size(); ++i) {
      const auto& [node, edgeTaken] = path[i];
      min_end = std::min(min_end, adjacency_list[node][edgeTaken]->contact->getEnd());
    }

    for (uint i = 0; i + 1 < path.size(); ++i) {
      const auto& [node, edgeTaken] = path[i];
      if (adjacency_list[node][edgeTaken]->contact->getEnd() == min_end) {
        adjacency_list[node][edgeTaken]->supressed = true;
        deleted_edges.push_back({ node, edgeTaken });
      }
    }

    routes.push_back(path_to_route(path));
  }

  for (const auto& [node, edgeTaken] : deleted_edges) {
    adjacency_list[node][edgeTaken]->supressed = false;
  }

  return routes;
}

std::vector<RouteT<Contact>> ContactPlan::first_ended(std::string from, std::string to) {
  // Assert from and to exists
  assert_participant_exists(from);
  assert_participant_exists(to);

  uint from_index = participant_to_identifier[from];
  uint to_index = participant_to_identifier[to];

  return cgr_first_ended(from_index, to_index);
}

std::vector<RouteT<Contact>> ContactPlan::cgr_first_depleted(uint from, uint to) {
  std::vector<RouteT<Contact>> routes;

  std::vector<std::pair<uint, uint>> deleted_edges;

  while (1) {
    std::pair<path, double> dijkstra_run = cgr_dijkstra(from, to);

    path path = dijkstra_run.first;

    if (path.size() == 0) {
      // No more routes
      break;
    }

    double min_volume = std::numeric_limits<double>::infinity();
    double currentCost = 0;

    for (uint i = 0; i + 1 < path.size(); ++i) {
      const auto& [node, edgeTaken] = path[i];
      double effective_volume = adjacency_list[node][edgeTaken]->contact->effectiveVolume(currentCost);
      currentCost = adjacency_list[node][edgeTaken]->contact->edgeCost(currentCost);
      min_volume = std::min(min_volume, effective_volume);
    }
    // Reset the current cost
    currentCost = 0;

    for (uint i = 0; i + 1 < path.size(); ++i) {
      const auto& [node, edgeTaken] = path[i];
      double effective_volume = adjacency_list[node][edgeTaken]->contact->effectiveVolume(currentCost);
      currentCost = adjacency_list[node][edgeTaken]->contact->edgeCost(currentCost);
      if (double_equal(effective_volume, min_volume)) {
        adjacency_list[node][edgeTaken]->supressed = true;
        deleted_edges.push_back({ node, edgeTaken });
      }
    }

    routes.push_back(path_to_route(path));
  }

  for (const auto& [node, edgeTaken] : deleted_edges) {
    adjacency_list[node][edgeTaken]->supressed = false;
  }

  return routes;
}

std::vector<RouteT<Contact>> ContactPlan::first_depleted(std::string from, std::string to) {
  // Assert from and to exists
  assert_participant_exists(from);
  assert_participant_exists(to);

  uint from_index = participant_to_identifier[from];
  uint to_index = participant_to_identifier[to];

  return cgr_first_depleted(from_index, to_index);
}