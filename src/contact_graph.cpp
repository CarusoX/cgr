#include "contact_graph.hpp"

bool double_equal(double a, double b, double EPS = 1e-9) {
  return abs(a - b) <= EPS;
}

bool double_less(double a, double b, double EPS = 1e-9) {
  return a + EPS < b;
}

std::vector<std::string> getContactParticipants(std::vector<Contact*>& contacts) {
  std::vector<std::string> contactParticipants;
  for (Contact* contact : contacts) {
    contactParticipants.push_back(contact->getFrom());
    contactParticipants.push_back(contact->getTo());
  }

  // Sort the contact names  
  std::sort(contactParticipants.begin(), contactParticipants.end());

  // We delete repeated names
  contactParticipants.resize(unique(contactParticipants.begin(), contactParticipants.end()) - contactParticipants.begin());

  return contactParticipants;
}

void ContactGraph::build_graph() {

  // Which contacts have i as "from"
  std::vector<std::vector<uint>> who_from(n);
  // Which contacts have i as "to"
  std::vector<std::vector<uint>> who_to(n);

  for (uint i = 0; i < n; ++i) {
    Contact* contact = contacts[i];
    uint from_index = std::lower_bound(contactParticipants.begin(), contactParticipants.end(), contact->getFrom()) - contactParticipants.begin();
    uint to_index = std::lower_bound(contactParticipants.begin(), contactParticipants.end(), contact->getTo()) - contactParticipants.begin();
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

  contactParticipants = getContactParticipants(_contacts);

  // For each contact participant, we will add a contact to itself
  for (std::string& contactParticipant : contactParticipants) {
    contacts.push_back(new Contact(contactParticipant, contactParticipant, 0, -1, 1, 0));
  }

  // Add the contacts from the user
  contacts.insert(contacts.end(), _contacts.begin(), _contacts.end());

  // Set the number of vertices of the graph
  n = contacts.size();

  // Resize the contact graph
  contact_graph.resize(n);

  build_graph();
}

void ContactGraph::cgr_dijkstra(uint from, uint to) {
  arrivalTime.resize(n);
  fill(arrivalTime.begin(), arrivalTime.end(), -1);
  std::priority_queue<std::pair<double, uint>> pq;

  arrivalTime[from] = 0;
  pq.push({ 0, from });
  while (!pq.empty()) {
    double currentArrivalTime = -pq.top().first;
    uint currentNode = pq.top().second;
    pq.pop();
    if (!double_equal(currentArrivalTime, arrivalTime[currentNode])) {
      continue;
    }

    for (uint neighbour : contact_graph[currentNode]) {
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
        pq.push({ -nextArrivalTime, neighbour });
      }
    }
  }
}

void ContactGraph::dijkstra(std::string from, std::string to) {
  uint from_index = std::lower_bound(contactParticipants.begin(), contactParticipants.end(), from) - contactParticipants.begin();
  uint to_index = std::lower_bound(contactParticipants.begin(), contactParticipants.end(), to) - contactParticipants.begin();

  if (from_index == n) {
    std::cerr << from << " is not a valid participant.\n";
    exit(-1);
  }
  if (to_index == n) {
    std::cerr << to << " is not a valid participant.\n";
    exit(-1);
  }

  cgr_dijkstra(from_index, to_index);
}