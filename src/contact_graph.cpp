#include "contact_graph.hpp"

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

void ContactGraph::add_participant_to_dictionary(std::string participant) {
  if (!participant_to_identifier.count(participant)) {
    participant_to_identifier[participant] = participant_to_identifier.size();
    participant_to_identity[participant] = contacts.size();
    contacts.push_back(new Contact(participant, participant, 0, -1, 1, 0));
  }
}

ContactGraph::ContactGraph(std::vector<Contact*> _contacts) {

  for (Contact* contact : _contacts) {
    add_participant_to_dictionary(contact->getFrom());
    add_participant_to_dictionary(contact->getTo());
  }

  contacts.insert(contacts.end(), _contacts.begin(), _contacts.end());

  // Set the number of vertices of the graph
  n = contacts.size();

  // Resize the contact graph
  contact_graph.resize(n);

  build_graph();
}

void ContactGraph::cgr_dijkstra(uint from, uint to) {
  arrivalTime.resize(n);
  prevnode.resize(n);
  prevedge.resize(n);
  fill(arrivalTime.begin(), arrivalTime.end(), -1);
  std::priority_queue<std::pair<double, uint>> pq;

  arrivalTime[from] = 0;
  pq.push({ 0, from });
  prevnode[0] = -1;
  prevedge[0] = -1;
  while (!pq.empty()) {
    double currentArrivalTime = -pq.top().first;
    uint currentNode = pq.top().second;
    pq.pop();
    if (!double_equal(currentArrivalTime, arrivalTime[currentNode])) {
      continue;
    }

    for(uint edge_index = 0; edge_index < contact_graph[currentNode].size(); ++edge_index) {
      uint neighbour = contact_graph[currentNode][edge_index];
      Contact* contact = contacts[neighbour];
      double nextArrivalTime;
      if (double_less(contact->getStart(), currentArrivalTime)) {
        nextArrivalTime = currentArrivalTime + contact->getOwlt();
      }
      else {
        nextArrivalTime = contact->getStart() + contact->getOwlt();
      }
      if (double_equal(arrivalTime[neighbour], -1) || double_less(nextArrivalTime, arrivalTime[neighbour])) {
        arrivalTime[neighbour] = nextArrivalTime;
        prevnode[neighbour] = currentNode;
        prevedge[neighbour] = edge_index;
        pq.push({ -nextArrivalTime, neighbour });
      }
    }
  }
}

void ContactGraph::dijkstra(std::string from, std::string to) {

  if (!participant_to_identity.count(from)) {
    std::cerr << from << " is not a valid participant.\n";
    exit(-1);
  }

  if (!participant_to_identity.count(to)) {
    std::cerr << to << " is not a valid participant.\n";
    exit(-1);
  }

  uint from_index = participant_to_identity[from];
  uint to_index = participant_to_identity[to];

  cgr_dijkstra(from_index, to_index);
}