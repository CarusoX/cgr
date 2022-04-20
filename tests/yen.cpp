#include <cassert>
#include "../src/contact_plan.hpp"
#include "../src/contact.hpp"
#include "../src/read.hpp"
#include "../src/route.hpp"

bool routeCompare(RouteT<Contact> &a, RouteT<Contact> &b) {
  return (*a) < b;
}

int main() {

  std::vector<ContactT> contacts = read_contact_plan("../plans/small.txt");

  ContactPlan* contactPlan = new ContactPlan(contacts);


  // Look for all the routes with dfs
  std::vector<RouteT<Contact>> dfsRoutes = contactPlan->dfs("1", "4");

  // Look for all the routes with yen
  std::vector<RouteT<Contact>> yenRoutes = contactPlan->yen("1", "4", dfsRoutes.size());

  sort(yenRoutes.begin(), yenRoutes.end(), routeCompare);

  assert(yenRoutes.size() == dfsRoutes.size());

  for(uint i = 0; i + 1 < dfsRoutes.size(); ++i) {
    assert(routeCompare(dfsRoutes[i], dfsRoutes[i + 1]));
  }

  for(uint i = 0; i < yenRoutes.size(); ++i) {
    assert((*yenRoutes[i]) == dfsRoutes[i]);
  }

  std::cout << "TEST PASSED!!" << std::endl;
}