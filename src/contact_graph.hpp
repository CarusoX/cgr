#ifndef CONTACT_GRAPH_H
#define CONTACT_GRAPH_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include "contact.hpp"
#include "graph.hpp"
#include "utilities.hpp"
#include "route.hpp"

class ContactGraph {

private:
  uint n; // the ammount of contacts (includes identity contacts)
  std::vector<ContactT> contacts; // the available contacts

  std::unordered_map<std::string, uint> participant_to_identifier; // participant to identifier
  std::unordered_map<std::string, uint> participant_to_identity; // participant to identity contact

  GraphT<Contact> graph = NULL;

  void add_participant_to_dictionary(std::string participant);

  void assert_participant_exists(std::string participant);

  void build_graph();

public:
  ContactGraph(std::vector<ContactT> contacts);

  Route* dijkstra(std::string from, std::string to);

  std::vector<Route*> yen(std::string from, std::string to, uint ammount);

  void debug() {
    std::cout << "Number of nodes " << n << "\n";
    std::cout << "Printing contact plan\n";
    for (ContactT contact : contacts) {
      contact->debug();
    }

    graph->debug();
  }
};

#endif
