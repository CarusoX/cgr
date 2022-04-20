#include <cassert>
#include "contact_plan.hpp"
#include "contact.hpp"
#include "read.hpp"
#include "route.hpp"

bool equal(RouteT<Contact> a, RouteT<Contact> b) {
  std::vector<ContactT> ca = a->getRoute();
  std::vector<ContactT> cb = b->getRoute();
  return ca == cb;
}

int main() {

  std::vector<ContactT> contacts = read_contact_plan("../plans/medium.txt");

  ContactPlan* contactPlan = new ContactPlan(contacts);
  // Route<Contact> *route = contactPlan->dijkstra("1", "5");
  // route->debug();
  std::vector<RouteT<Contact>> yenRoutes = contactPlan->yen("1", "4", 20);
  std::cout << "YEN ROUTES\n============" << std::endl;
  std::cout << yenRoutes.size() << std::endl;
  // for (RouteT<Contact> route : yenRoutes) {
  //   route->debug();
  //   std::cout << "============" << std::endl;
  // }

  // std::vector<RouteT<Contact>> dfsRoutes = contactPlan->dfs("1", "4");
  // std::cout << "DFS ROUTES\n============" << std::endl;
  // std::cout << dfsRoutes.size() << std::endl;
  // for (RouteT<Contact> route : dfsRoutes) {
  //   route->debug();
  //   std::cout << "============" << std::endl;
  // }
  return 0;
}