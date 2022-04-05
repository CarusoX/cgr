#include "contact_graph.hpp"
#include "contact.hpp"
#include "read.hpp"
#include "route.hpp"

int main() {

  std::vector<Contact*> contacts = read_contact_plan("../plans/tutorial.txt");

  ContactGraph* graph = new ContactGraph(contacts);
  Route* bestRoute = graph->dijkstra("1", "2");

  bestRoute->debug();

  return 0;
}