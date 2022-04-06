#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include "contact.hpp"
#include "utilities.hpp"

class Route {

private:
  std::vector<ContactT> route;
  double routeCost;

  // -----------------------------------------------------------------------------
  // Hashing related stuff
  // -----------------------------------------------------------------------------
  __uint128_t MOD = 212345678987654321LL, P = 1777771, PI = 106955741089659571LL;
  std::vector<__uint128_t> incrementalH, incrementalP, incrementalPI;

  void pushOneHashingInformation(uintptr_t pointer);

  void precomputeHashingInformation();

public:
  Route(std::vector<ContactT> _route, double _routeCost);

  Route* addContact(ContactT contact);

  std::vector<ContactT> getRoute();

  std::vector<ContactT> getPrefixRoute(uint p);

  double getRouteCost();

  __uint128_t getHash(uint s, uint e) {
    return (((incrementalH[e] - incrementalH[s] + MOD) % MOD) * incrementalPI[s]) % MOD;
  }

  void debug() {
    for (ContactT contact : route) {
      contact->debug();
    }
    std::cout << "Route cost: " << routeCost << std::endl;
  }
};

#endif