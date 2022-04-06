#include "route.hpp"

void Route::pushOneHashingInformation(uintptr_t pointer) {
  incrementalH.push_back((incrementalH.back() + incrementalP.back() * pointer) % MOD);
  incrementalPI.push_back((incrementalPI.back() * PI) % MOD);
  incrementalP.push_back((incrementalP.back() * P) % MOD);
}

void Route::precomputeHashingInformation() {
  incrementalH.push_back(0);
  incrementalP.push_back(1);
  incrementalPI.push_back(1);

  for(uint i = 1; i < route.size() + 1; ++i) {
    uintptr_t pointer = reinterpret_cast<uintptr_t>(route[i - 1]);
    pushOneHashingInformation(pointer);
  }
}

Route::Route(std::vector<ContactT> _route, double _routeCost) {
  route = _route;
  routeCost = _routeCost;
  precomputeHashingInformation();
}

Route* Route::addContact(ContactT contact) {
  if(route.size()) {
    // TODO -- @carusox refactor this into Contact class
    if (double_less(contact->getStart(), routeCost)) {
      // If the contact is already active, just add the owlt
      routeCost += contact->getOwlt();
    } else {
      routeCost = contact->getStart() + contact->getOwlt();
    }
  }
  route.push_back(contact);
  uintptr_t pointer = reinterpret_cast<uintptr_t>(contact);
  pushOneHashingInformation(pointer);
  return this;
}

std::vector<ContactT> Route::getRoute() {
  return route;
}

std::vector<ContactT> Route::getPrefixRoute(uint p) {
  std::vector<ContactT> preffixRoute;
  for(uint i = 0; i < p; ++i) {
    preffixRoute.push_back(route[i]);
  }
  return preffixRoute;
}

double Route::getRouteCost() {
  return routeCost;
}