#include "contact_plan.hpp"
#include "contact.hpp"
#include "read.hpp"
#include "route.hpp"

int main() {

  std::vector<ContactT> contacts = read_contact_plan("../plans/tutorial.txt");

  ContactPlan* contactPlan = new ContactPlan(contacts);
  Route<Contact> *route = contactPlan->dijkstra("1", "5");
  route->debug();

  return 0;
}