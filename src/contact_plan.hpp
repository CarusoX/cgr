#ifndef CONTACT_PLAN_H
#define CONTACT_PLAN_H

#include <iostream>
#include <unordered_map>
#include <vector>
#include "contact.hpp"
#include "graph.hpp"
#include "utilities.hpp"
#include "route.hpp"

class ContactPlan {

private:
  uint n; // the ammount of contacts (includes identity contacts)
  std::vector<ContactT> contacts; // the available contacts

  std::unordered_map<std::string, uint> participant_to_identifier; // participant to identifier
  std::unordered_map<std::string, uint> participant_to_identity; // participant to identity contact

  std::vector<uint> identities;

  GraphT<Contact> graph = NULL;

  void add_participant_to_dictionary(std::string participant);

  void assert_participant_exists(std::string participant);

  void build_graph();

public:
  ContactPlan(std::vector<ContactT> contacts);

  RouteT<Contact> dijkstra(std::string from, std::string to);

  std::vector<RouteT<Contact>> yen(std::string from, std::string to, uint ammount);

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
