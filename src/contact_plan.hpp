#ifndef CONTACT_PLAN_H
#define CONTACT_PLAN_H

#include <chrono>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <vector>
#include "contact.hpp"
#include "utilities.hpp"
#include "route.hpp"

struct Edge {
  uint to;
  ContactT contact;
  bool supressed;
  Edge(uint _to, ContactT _contact) : to(_to), contact(_contact), supressed(false) {}
};

using path = std::vector<std::pair<uint, uint>>;

class ContactPlan {

private:
  uint n; // the ammount of particpants
  std::vector<ContactT> contacts; // the available contacts

  std::unordered_map<std::string, uint> participant_to_identifier; // participant to identifier

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

  RouteT<Contact> path_to_route(path);

  std::pair<path, double> cgr_dijkstra(uint from, uint to, double start = 0);

  // ---------------------------------------------------------------------------
  // Yen related stuff
  // ---------------------------------------------------------------------------=
  std::vector<bool> supressed;

  std::vector<RouteT<Contact>> cgr_yen(uint from, uint to, uint ammount);
  
  // ---------------------------------------------------------------------------
  // DFS related stuff
  // ---------------------------------------------------------------------------
  path dfs_stack;
  std::vector<bool> dfs_visited;
  void cgr_dfs(uint where, uint to, std::vector<RouteT<Contact>> &routes, double currentTime = 0);

  // ---------------------------------------------------------------------------
  // First ended related stuff
  // ---------------------------------------------------------------------------

  std::vector<RouteT<Contact>> cgr_first_ended(uint from, uint to);

public:
  ContactPlan(std::vector<ContactT> contacts);

  RouteT<Contact> dijkstra(std::string from, std::string to);

  std::vector<RouteT<Contact>> yen(std::string from, std::string to, uint ammount);

  std::vector<RouteT<Contact>> dfs(std::string from, std::string to);

  std::vector<RouteT<Contact>> first_ended(std::string from, std::string to);

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
