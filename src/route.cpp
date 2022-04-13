#include "route.hpp"

template <class T>
Route<T>::Route() {
  route = {};
  routeCost = 0;
}

template <class T>
Route<T>::Route(std::vector<T*> _route, double _routeCost) {
  route = _route;
  routeCost = _routeCost;
}

template <class T>
Route<T>::~Route() {
  while (route.size()) {
    delete route.back();
    route.pop_back();
  }
}

template <class T>
bool Route<T>::isValid() {
  return !double_equal(-1, routeCost);
}

template <class T>
RouteT<T> Route<T>::addNode(T* node) {
  if (route.size()) {
    routeCost = route.back()->edgeCost(node, routeCost);
  }
  route.push_back(node);
  return this;
}

template <class T>
RouteT<T> Route<T>::merge(RouteT<T> otherRoute) {
  std::vector<T*> nodes = otherRoute->getRoute();
  for(T* node : nodes) {
    this->addNode(node);
  }
  return this;
}

template <class T>
std::vector<T*> Route<T>::getRoute() {
  return route;
}

template <class T>
T* Route<T>::getNode(uint p) {
  if (route.size() <= p) {
    std::cerr << "Node at position " << p << " does not exist" << std::endl;
    exit(-1);
  }
  return route[p];
}

template <class T>
double Route<T>::getRouteCost() {
  return routeCost;
}

template class Route<Contact>;