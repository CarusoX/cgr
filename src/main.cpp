#include "contact_plan.hpp"
#include "contact.hpp"
#include "read.hpp"
#include "route.hpp"

int main() {

  std::vector<ContactT> contacts = read_contact_plan("../plans/tutorial.txt");

  ContactPlan* contactPlan = new ContactPlan(contacts);
  Route<Contact> *route = contactPlan->dijkstra("1", "5");
  // route->debug();
  std::vector<RouteT<Contact>> yenRoutes = contactPlan->yen("1", "5", 5);
  for(RouteT<Contact> route : yenRoutes) {
    route->debug();
    std::cout << "============" << std::endl;
  }

  return 0;
}