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
  Route();
  Route(std::vector<T*> _route, double _routeCost);

  ~Route();

  bool isValid();

  Route<T>* addNode(T* contact);

  Route<T>* merge(Route<T>* otherRoute);

  std::vector<T*> getRoute() const;

  T* getNode(uint p);

  double getRouteCost() const;

  bool operator<(const Route<T>* otherRoute);

  bool operator==(const Route<T>* otherRoute);

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