#include "route.hpp"

Route::Route(std::vector<Contact*> _route, double _routeCost) {
  route = _route;
  routeCost = _routeCost;
}

std::vector<Contact*> Route::getRoute() {
  return route;
}

double Route::getRouteCost() {
  return routeCost;
}