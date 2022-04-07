#include "route.hpp"

template <class T>
Route<T>::Route(std::vector<T*> _route, double _routeCost) {
  route = _route;
  routeCost = _routeCost;
}

template <class T>
RouteT<T> Route<T>::addNode(T* contact) {
  if (route.size()) {
    routeCost = route.back()->edgeCost(contact, routeCost);
  }
  route.push_back(contact);
  return this;
}

template <class T>
std::vector<T*> Route<T>::getRoute() {
  return route;
}

template <class T>
std::vector<T*> Route<T>::getPrefixRoute(uint p) {
  std::vector<T*> preffixRoute;
  for (uint i = 0; i < p; ++i) {
    preffixRoute.push_back(route[i]);
  }
  return preffixRoute;
}

template <class T>
double Route<T>::getRouteCost() {
  return routeCost;
}

template class Route<Contact>;