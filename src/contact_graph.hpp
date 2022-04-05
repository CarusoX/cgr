#ifndef CONTACT_GRAPH_H
#define CONTACT_GRAPH_H

#include <algorithm>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include "contact.hpp"
#include "utilities.hpp"
#include "route.hpp"

class ContactGraph {

private:
  uint n; // the ammount of contacts of the graph (includes identity contacts)
  std::vector<Contact*> contacts; // the contacts of the graph
  std::vector<std::vector<uint>> contact_graph; // graph represented by an adjacency list

  std::unordered_map<std::string, uint> participant_to_identifier; // participant to identifier
  std::unordered_map<std::string, uint> participant_to_identity; // participant to identity contact

  void add_participant_to_dictionary(std::string participant);

  void build_graph();

  // ---------------------------------------------------------------------------
  // Dijkstra related stuff
  // ---------------------------------------------------------------------------
  std::vector<double> arrivalTime;
  std::vector<uint> prevnode;
  std::vector<uint> prevedge;

  Route* cgr_dijkstra(uint from, uint to);

  // ---------------------------------------------------------------------------
  // Yen related stuff
  // ---------------------------------------------------------------------------

public:
  ContactGraph(std::vector<Contact*> contacts);

  Route* dijkstra(std::string from, std::string to);

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
