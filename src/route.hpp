#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include "contact.hpp"
#include "utilities.hpp"

class Route {

  std::vector<Contact*> route;
  double routeCost;

public:
  Route(std::vector<Contact*> _route, double _routeCost);

  std::vector<Contact*> getRoute();

  double getRouteCost();

  void debug() {
    for(Contact* contact : route) {
      contact->debug();
    }
    std::cout << "Route cost: " << routeCost << std::endl;
  }
};

#endif