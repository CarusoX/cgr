#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include "contact.hpp"
#include "utilities.hpp"

// template <class T, class S>
// concept CheckType = std::is_base_of<S, T>::value;

template <class T>
class Route {

private:
  std::vector<T*> route;
  double routeCost;

public:
  Route(std::vector<T*> _route, double _routeCost);

  Route<T>* addNode(T* contact);

  std::vector<T*> getRoute();

  std::vector<T*> getPrefixRoute(uint p);

  double getRouteCost();

  void debug() {
    for (T* node : route) {
      node->debug();
    }
    std::cout << "Route cost: " << routeCost << std::endl;
  }
};

template <class T>
using RouteT = Route<T>*;

#endif