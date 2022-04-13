#ifndef CONTACT_PLAN_H
#define CONTACT_PLAN_H

#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
#include "contact.hpp"
#include "utilities.hpp"
#include "route.hpp"

struct Edge {
  uint to;
  bool supressed;
  Edge(uint _to) : to(_to), supressed(false) {}
};

class ContactPlan {

private:
  uint n; // the ammount of contacts (includes identity contacts)
  uint p; // the ammount of participants
  std::vector<ContactT> contacts; // the available contacts

  std::unordered_map<std::string, uint> participant_to_identifier; // participant to identifier
  std::unordered_map<std::string, uint> participant_to_identity; // participant to identity contact

  void add_participant_to_dictionary(std::string participant);

  void assert_participant_exists(std::string participant);

  // ---------------------------------------------------------------------------
  // Graph related stuff
  // ---------------------------------------------------------------------------

  std::vector<std::vector<Edge*>> adjacency_list;

  void build_graph();

  // ---------------------------------------------------------------------------
  // Dijkstra related stuff
  // ---------------------------------------------------------------------------
  std::vector<double> arrivalTime;
  std::vector<uint> prevnode;
  std::vector<uint> prevedge;

  void prepare_working_area();

  RouteT<Contact> cgr_dijkstra(uint from, uint to);

  // ---------------------------------------------------------------------------
  // Yen related stuff
  // ---------------------------------------------------------------------------=
  std::vector<bool> supressed;
  std::unordered_map<ContactT, uint> contact_to_identifier; // contact to identifier
  std::vector<std::unordered_map<uint, uint>> edge_identifier; // edge -> identifier

  std::vector<RouteT<Contact>> cgr_yen(uint from, uint to, uint ammount);

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

    // TODO -- @carusox debug graph
  }
};

#endif
