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

  if (graph != NULL) return;

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

  std::vector<std::vector<uint>> edges(n);

  // Now we can build the edges
  for (uint i = 0; i < n; ++i) {
    std::vector<uint>& who_from_i = who_from[i];
    std::vector<uint>& who_to_i = who_to[i];

    for (uint x : who_to_i) for (uint y : who_from_i) {
      if (x != y) { // avoid identity edges
        edges[x].push_back(y);
      }
    }
  }

  // Create the graph class
  graph = new Graph(contacts, edges);
}

ContactGraph::ContactGraph(std::vector<ContactT> _contacts) {

  // We assign each participant a new identifier
  for (ContactT contact : _contacts) {
    add_participant_to_dictionary(contact->getFrom());
    add_participant_to_dictionary(contact->getTo());
  }

  // Insert the contacts
  contacts.insert(contacts.end(), _contacts.begin(), _contacts.end());

  // Set the number of vertices of the graph
  n = contacts.size();
}

Route* ContactGraph::dijkstra(std::string from, std::string to) {

  // Build the graph if not done yet
  build_graph();

  // Assert from and to exists
  assert_participant_exists(from);
  assert_participant_exists(to);

  uint from_index = participant_to_identity[from];
  uint to_index = participant_to_identity[to];

  return graph->dijkstra(from_index, to_index);
}

// std::vector<Route*> ContactGraph::cgr_yen(uint from, uint to, uint ammount) {
//   std::vector<Route*> routes;

//   Route* bestRoute = cgr_dijkstra(from, to);


//   if (double_equal(bestRoute->getRouteCost(), -1)) {
//     // There are no roots in this case
//     return routes;
//   }

//   // Push the first route to the list
//   routes.push_back(bestRoute);

//   // Will represent a pair of {c, r}, meaning we have a route "r" with cost "c"
//   typedef std::pair<double, Route*> state;

//   // Create a minimum heap for the routes
//   std::priority_queue<state, std::vector<state>, std::greater<state>> pq;

//   for (uint k = 1; k < ammount; ++k) {
//     // Get the previous best route
//     std::vector<ContactT> lastRoute = routes[k - 1]->getRoute();

//     Route* rootRoute = new Route({}, 0);

//     for (uint i = 0; i + 2 < lastRoute.size(); ++i) {

//       ContactT spurContact = lastRoute[i];

//       rootRoute->addContact(spurContact);

//       for (Route* route : routes) {
//         if (rootRoute->getHash(0, i + 1) == route->getHash(0, i + 1)) {
//           std::cout << i << " works " << std::endl;
//         }
//       }
//     }
//     break;
//   }
// }

std::vector<Route*> ContactGraph::yen(std::string from, std::string to, uint ammount) {

  // Build the graph if not done yet
  build_graph();

  // Assert from and to exists
  assert_participant_exists(from);
  assert_participant_exists(to);

  uint from_index = participant_to_identity[from];
  uint to_index = participant_to_identity[to];

  return graph->yen(from_index, to_index, ammount);
}