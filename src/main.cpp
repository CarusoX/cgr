#include "read.hpp"
#include "contact.hpp"
#include "contact_graph.hpp"

int main() {

  std::vector<Contact*> contacts = read_contact_plan("../plans/tutorial.txt");

  ContactGraph* graph = new ContactGraph(contacts);
  graph->dijkstra("1", "5");

  return 0;
}