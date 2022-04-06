#include "contact_plan.hpp"
#include "contact.hpp"
#include "read.hpp"
#include "route.hpp"

int main() {

  std::vector<ContactT> contacts = read_contact_plan("../plans/tutorial.txt");

  ContactPlan* graph = new ContactPlan(contacts);
  graph->yen("1", "5", 3);

  return 0;
}