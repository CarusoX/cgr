#include "contact_plan.hpp"

void ContactPlan::add_participant_to_dictionary(std::string participant) {
  if (!participant_to_identifier.count(participant)) {
    // We assign the participant a new identifier
    participant_to_identifier[participant] = participant_to_identifier.size();

    // We save the index of the identity contact for the participant
    participant_to_identity[participant] = contacts.size();

    // Push the identity contact
    contacts.push_back(new Contact(participant, participant, 0, -1, 1, 0));
  }
}

void ContactPlan::assert_participant_exists(std::string participant) {
  if (!participant_to_identity.count(participant)) {
    std::cerr << participant << " is not a valid participant.\n";
    exit(-1);
  }
}

void ContactPlan::build_graph() {

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

ContactPlan::ContactPlan(std::vector<ContactT> _contacts) {

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

RouteT<Contact> ContactPlan::dijkstra(std::string from, std::string to) {

  // Build the graph if not done yet
  build_graph();

  // Assert from and to exists
  assert_participant_exists(from);
  assert_participant_exists(to);

  uint from_index = participant_to_identity[from];
  uint to_index = participant_to_identity[to];

  return graph->dijkstra(from_index, to_index);
}

std::vector<RouteT<Contact>> ContactPlan::yen(std::string from, std::string to, uint ammount) {

  // Build the graph if not done yet
  build_graph();

  // Assert from and to exists
  assert_participant_exists(from);
  assert_participant_exists(to);

  uint from_index = participant_to_identity[from];
  uint to_index = participant_to_identity[to];

  return graph->yen(from_index, to_index, ammount);
}