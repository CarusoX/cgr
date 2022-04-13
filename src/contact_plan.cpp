#include "contact_plan.hpp"

void ContactPlan::add_participant_to_dictionary(std::string participant) {
  if (!participant_to_identifier.count(participant)) {
    // We assign the participant a new identifier
    participant_to_identifier[participant] = participant_to_identifier.size();

    // We save the index of the identity contact for the participant
    participant_to_identity[participant] = contacts.size();

    // Push the identity contact
    ContactT identityContact = new Contact(participant, participant, 0, -1, 1, 0);
    contact_to_identifier[identityContact] = contacts.size();
    contacts.push_back(identityContact);
  }
}

void ContactPlan::assert_participant_exists(std::string participant) {
  if (!participant_to_identity.count(participant)) {
    std::cerr << participant << " is not a valid participant.\n";
    exit(-1);
  }
}

void ContactPlan::build_graph() {

  // Which contacts have i as "from"
  std::vector<std::vector<uint>> who_from(n);
  // Which contacts have i as "to"
  std::vector<std::vector<uint>> who_to(n);

  for (uint i = 0; i < n; ++i) {
    ContactT contact = contacts[i];
    uint from_index = participant_to_identifier[contact->getFrom()];
    uint to_index = participant_to_identifier[contact->getTo()];
    who_from[from_index].push_back(i);
    who_to[to_index].push_back(i);
  }

  adjacency_list.resize(n);
  edge_identifier.resize(n);

  // Now we can build the edges
  for (uint i = 0; i < n; ++i) {
    std::vector<uint>& who_from_i = who_from[i];
    std::vector<uint>& who_to_i = who_to[i];

    for (uint x : who_to_i) for (uint y : who_from_i) {
      if (x != y) { // avoid identity edges
        edge_identifier[x][y] = adjacency_list[x].size();
        adjacency_list[x].push_back(new Edge(y));
      }
    }
  }
}

ContactPlan::ContactPlan(std::vector<ContactT> _contacts) {

  // We assign each participant a new identifier
  for (ContactT contact : _contacts) {
    add_participant_to_dictionary(contact->getFrom());
    add_participant_to_dictionary(contact->getTo());
  }

  // Insert the contacts
  for (ContactT contact : _contacts) {
    contact_to_identifier[contact] = contacts.size();
    contacts.push_back(contact);
  }

  // Set the number of contacts
  n = contacts.size();

  // Set the number of participants
  p = participant_to_identifier.size();

  // Build the graph
  build_graph();
}

void ContactPlan::prepare_working_area() {
  // Initialize arrival times to -1
  arrivalTime.resize(p);
  fill(arrivalTime.begin(), arrivalTime.end(), -1);

  supressed.resize(n, false);
  prevnode.resize(n);
  prevedge.resize(n);
}


RouteT<Contact> ContactPlan::cgr_dijkstra(uint from, uint to) {
  // Will represent a pair of { c, n }, meaning we have a path of cost "c" to node "n"
  typedef std::pair<double, uint> state;
  // Create a minimum heap
  std::priority_queue<state, std::vector<state>, std::greater<state>> pq;

  std::string rootParticipant = contacts[from]->getTo();
  std::string terminalParticipant = contacts[to]->getTo();

  // From node setup
  arrivalTime[participant_to_identifier[rootParticipant]] = 0;
  pq.push({ 0, from });
  prevnode[from] = -1;
  prevedge[from] = -1;

  uint lastNeededContact = -1;

  while (!pq.empty()) {
    // Get the best state so far
    double currentArrivalTime = pq.top().first;
    uint current = pq.top().second;
    ContactT currentContact = contacts[current];
    uint currentParticipant = participant_to_identifier[currentContact->getTo()];

    pq.pop();

    // If this isn't the real best path to the node, skip it
    if (!double_equal(currentArrivalTime, arrivalTime[currentParticipant])) {
      continue;
    }

    if (currentParticipant == participant_to_identifier[terminalParticipant]) {
      lastNeededContact = current;
      break;
    }

    // Traverse all the edges
    for (uint edge_index = 0; edge_index < adjacency_list[current].size(); ++edge_index) {
      Edge* edge = adjacency_list[current][edge_index];

      // If the edge is supressed, don't go there
      if (edge->supressed) continue;


      uint neighbour = edge->to;
      ContactT nextContact = contacts[neighbour];
      uint nextParticipant = participant_to_identifier[nextContact->getTo()];

      // If there is no way to go the next node, or it's supressed, continue
      if (!currentContact->canGoTo(nextContact, currentArrivalTime) || supressed[neighbour]) {
        continue;
      }

      double nextArrivalTime = currentContact->edgeCost(nextContact, currentArrivalTime);

      if (double_equal(arrivalTime[nextParticipant], -1) || double_less(nextArrivalTime, arrivalTime[nextParticipant])) {
        // If this contact was not reached yet, or we found a better path, update accordingly
        arrivalTime[nextParticipant] = nextArrivalTime;
        prevnode[neighbour] = current;
        prevedge[neighbour] = edge_index;
        pq.push({ nextArrivalTime, neighbour });
      }
    }
  }


  // If we did not find a path
  if (lastNeededContact == uint(-1)) {
    return new Route<Contact>({}, -1);
  }

  // Reconstruct the route backwards
  std::vector<ContactT> route;
  uint current = lastNeededContact;
  while (current != uint(-1)) {
    route.push_back(contacts[current]);
    current = prevnode[current];
  }
  // Reverse the route
  reverse(route.begin(), route.end());

  return new Route(route, arrivalTime[participant_to_identifier[terminalParticipant]]);
}

RouteT<Contact> ContactPlan::dijkstra(std::string from, std::string to) {

  // Assert from and to exists
  assert_participant_exists(from);
  assert_participant_exists(to);

  uint from_index = participant_to_identity[from];
  uint to_index = participant_to_identity[to];

  prepare_working_area();

  return cgr_dijkstra(from_index, to_index);
}

std::vector<RouteT<Contact>> ContactPlan::cgr_yen(uint from, uint to, uint ammount) {
  std::vector<RouteT<Contact>> routes;

  prepare_working_area();
  RouteT<Contact> bestRoute = cgr_dijkstra(from, to);


  if (!bestRoute->isValid()) {
    // There are no routes in this case
    return routes;
  }

  // Push the first route to the list
  routes.push_back(bestRoute);

  // Will represent a pair of {c, r}, meaning we have a route "r" with cost "c"
  typedef std::pair<double, RouteT<Contact>> state;

  // Create a minimum heap for the routes
  std::priority_queue<state, std::vector<state>, std::greater<state>> pq;

  for (uint k = 1; k < ammount; ++k) {
    // Get the previous best route
    std::vector<ContactT> lastRoute = routes[k - 1]->getRoute();

    RouteT<Contact> rootRoute = new Route<Contact>();

    // Initialize the matching routes
    std::vector<uint> matching_routes, next_matching_routes;
    for (uint i = 0; i < k; ++i) {
      matching_routes.push_back(i);
    }

    for (uint i = 0; i + 1 < lastRoute.size(); ++i) {
      std::vector<std::pair<uint, uint>> deleted_edges;

      ContactT spurContact = lastRoute[i];
      uint spurIdentifier = contact_to_identifier[spurContact];

      // Recalculate the matching routes based on the last vertex
      for (uint matching_route : matching_routes) {
        if (routes[matching_route]->getNode(i) == spurContact) {
          // We need to delete the edge with the next node
          uint next_node_id = contact_to_identifier[routes[matching_route]->getNode(i + 1)];
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


      prepare_working_area();
      for(uint j = 0; j < i; ++j) {
        // We don't want to come back to these particpants
        arrivalTime[participant_to_identifier[rootRoute->getNode(j)->getTo()]] = 0;
      }

      // Calculate the spur path from the spur node to the sink
      RouteT<Contact> spurPath = cgr_dijkstra(spurIdentifier, to);

      if (spurPath->isValid()) {
        RouteT<Contact> totalPath = new Route<Contact>();
        totalPath->merge(rootRoute)->merge(spurPath);
        pq.push({ totalPath->getRouteCost(), totalPath });
      }

      // Now the spur node is fixated
      rootRoute->addNode(spurContact);

      // Supress it for next time
      supressed[spurIdentifier] = true;

      // Restore the edges
      for (std::pair<uint, uint> edge : deleted_edges) {
        adjacency_list[edge.first][edge.second]->supressed = false;
      }
    }

    // Restore the nodes
    for (uint i = 0; i + 1 < lastRoute.size(); ++i) {
      uint node_id = contact_to_identifier[rootRoute->getNode(i)];
      supressed[node_id] = false;
    }
    if (pq.empty()) {
      // No other paths available
      break;
    }
    routes.push_back(pq.top().second);
    pq.pop();
  }

  return routes;
}

std::vector<RouteT<Contact>> ContactPlan::yen(std::string from, std::string to, uint ammount) {

  // Assert from and to exists
  assert_participant_exists(from);
  assert_participant_exists(to);

  uint from_index = participant_to_identity[from];
  uint to_index = participant_to_identity[to];

  return cgr_yen(from_index, to_index, ammount);
}