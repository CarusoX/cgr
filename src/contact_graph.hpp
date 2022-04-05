#ifndef CONTACT_GRAPH_H
#define CONTACT_GRAPH_H

#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include "contact.hpp"
#include "utilities.hpp"

class ContactGraph {

private:
  uint n;
  std::vector<Contact*> contacts;
  std::vector<std::vector<uint>> contact_graph;
  std::unordered_map<std::string, uint> participant_to_identifier;
  std::unordered_map<std::string, uint> participant_to_identity;
  std::vector<uint> prevnode;
  std::vector<uint> prevedge;

  void add_participant_to_dictionary(std::string participant);

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
