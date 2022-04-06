#include "contact_graph.hpp"

void ContactGraph::add_participant_to_dictionary(std::string participant) {
  if (!participant_to_identifier.count(participant)) {
    // We assign the participant a new identifier
    participant_to_identifier[participant] = participant_to_identifier.size();

    // We save the index of the identity contact for the participant
    participant_to_identity[participant] = contacts.size();

    // Push the identity contact
    contacts.push_back(new Contact(participant, participant, 0, -1, 1, 0));
  }
}

void ContactGraph::assert_participant_exists(std::string participant) {
  if (!participant_to_identity.count(participant)) {
    std::cerr << participant << " is not a valid participant.\n";
    exit(-1);
  }
}

void ContactGraph::build_graph() {

  // Which contacts have i as "from"
  std::vector<std::vector<uint>> who_from(n);
  // Which contacts have i as "to"
  std::vector<std::vector<uint>> who_to(n);

  for (uint i = 0; i < n; ++i) {
    Contact* contact = contacts[i];
    uint from_index = participant_to_identifier[contact->getFrom()];
    uint to_index = participant_to_identifier[contact->getTo()];
    who_from[from_index].push_back(i);
    who_to[to_index].push_back(i);
  }

  // Now we can build the graph
  for (uint i = 0; i < n; ++i) {
    std::vector<uint>& who_from_i = who_from[i];
    std::vector<uint>& who_to_i = who_to[i];

    for (uint x : who_to_i) for (uint y : who_from_i) {
      if (x != y) { // avoid identity edges
        contact_graph[x].push_back(y);
      }
    }
  }
}

ContactGraph::ContactGraph(std::vector<Contact*> _contacts) {

  // We assign each participant a new identifier
  for (Contact* contact : _contacts) {
    add_participant_to_dictionary(contact->getFrom());
    add_participant_to_dictionary(contact->getTo());
  }

  // Insert the contacts
  contacts.insert(contacts.end(), _contacts.begin(), _contacts.end());

  // Set the number of vertices of the graph
  n = contacts.size();

  // Resize the contact graph
  contact_graph.resize(n);

  // Finish building the graph
  build_graph();
}

Route* ContactGraph::cgr_dijkstra(uint from, uint to) {
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
    uint currentNode = pq.top().second;
    pq.pop();

    // If this isn't the real best path to the node, skip it
    if (!double_equal(currentArrivalTime, arrivalTime[currentNode])) {
      continue;
    }

    // Traverse all the edges
    for (uint edge_index = 0; edge_index < contact_graph[currentNode].size(); ++edge_index) {
      uint neighbour = contact_graph[currentNode][edge_index];
      Contact* contact = contacts[neighbour];
      double nextArrivalTime;

      if (double_less(contact->getEnd(), currentArrivalTime)) {
        // If this contact is not reachable skip it
        continue;
      }
      else if (double_less(contact->getStart(), currentArrivalTime)) {
        // If the contact is already active, just add the owlt
        nextArrivalTime = currentArrivalTime + contact->getOwlt();
      }
      else {
        // Otherwise, we need to wait until the contact is active, and then add the owlt
        nextArrivalTime = contact->getStart() + contact->getOwlt();
      }

      if (double_equal(arrivalTime[neighbour], -1) || double_less(nextArrivalTime, arrivalTime[neighbour])) {
        // If this contact was not reached yet, or we found a better path, update accordingly
        arrivalTime[neighbour] = nextArrivalTime;
        prevnode[neighbour] = currentNode;
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
  std::vector<Contact*> route;
  uint current = to;
  while (current != -1) {
    route.push_back(contacts[current]);
    current = prevnode[current];
  }
  // Reverse the route
  reverse(route.begin(), route.end());

  return new Route(route, arrivalTime[to]);
}

Route* ContactGraph::dijkstra(std::string from, std::string to) {
  // Assert from and to exists
  assert_participant_exists(from);
  assert_participant_exists(to);

  uint from_index = participant_to_identity[from];
  uint to_index = participant_to_identity[to];

  return cgr_dijkstra(from_index, to_index);
}

std::vector<Route*> ContactGraph::cgr_yen(uint from, uint to, uint ammount) {
  std::vector<Route*> routes;

  Route* bestRoute = cgr_dijkstra(from, to);


  if (double_equal(bestRoute->getRouteCost(), -1)) {
    // There are no roots in this case
    return routes;
  }

  // Push the first route to the list
  routes.push_back(bestRoute);

  // Will represent a pair of {c, r}, meaning we have a route "r" with cost "c"
  typedef std::pair<double, Route*> state;

  // Create a minimum heap for the routes
  std::priority_queue<state, std::vector<state>, std::greater<state>> pq;

  for (uint k = 1; k < ammount; ++k) {
    // Get the previous best route
    std::vector<Contact*> lastRoute = routes[k - 1]->getRoute();

    Route* rootRoute = new Route({}, 0);

    for (uint i = 0; i + 2 < lastRoute.size(); ++i) {

      Contact* spurContact = lastRoute[i];

      rootRoute->addContact(spurContact);

      for (Route* route : routes) {
        if (rootRoute->getHash(0, i + 1) == route->getHash(0, i + 1)) {
          std::cout << i << " works " << std::endl;
        }
      }
    }
    break;
  }
}

std::vector<Route*> ContactGraph::yen(std::string from, std::string to, uint ammount) {
  // Assert from and to exists
  assert_participant_exists(from);
  assert_participant_exists(to);

  uint from_index = participant_to_identity[from];
  uint to_index = participant_to_identity[to];

  return cgr_yen(from_index, to_index, ammount);
}