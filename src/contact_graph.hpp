#ifndef CONTACT_GRAPH_H
#define CONTACT_GRAPH_H

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
#include "contact.hpp"

class ContactGraph {

private:
  uint n;
  std::vector<Contact*> contacts;
  std::vector<std::vector<uint>> contact_graph;
  std::vector<std::string> contactParticipants;
  void build_graph();

  // ---------------------------------------------------------------------------
  // Dijkstra related stuff
  // ---------------------------------------------------------------------------
  std::vector<double> arrivalTime;

  void cgr_dijkstra(uint from, uint to);

public:
  ContactGraph(std::vector<Contact*> contacts);

  void dijkstra(std::string from, std::string to);

  void debug() {
    std::cout << "Number of nodes " << n << "\n";
    std::cout << "Printing contacts\n";
    for (Contact* contact : contacts) {
      contact->debug();
    }

    std::cout << "Printing graph\n";
    for (uint i = 0; i < n; ++i) {
      std::cout << "Neighbours of " << i << ": ";
      for (uint x : contact_graph[i]) std::cout << x << ", ";
      std::cout << "\n";
    }
  }
};

#endif
