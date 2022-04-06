#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include "contact.hpp"
#include "utilities.hpp"

class Route {

private:
  std::vector<Contact*> route;
  double routeCost;

  // -----------------------------------------------------------------------------
  // Hashing related stuff
  // -----------------------------------------------------------------------------
  __uint128_t MOD = 212345678987654321LL, P = 1777771, PI = 106955741089659571LL;
  std::vector<__uint128_t> incrementalH, incrementalP, incrementalPI;

  void pushOneHashingInformation(uintptr_t pointer);

  void precomputeHashingInformation();

public:
  Route(std::vector<Contact*> _route, double _routeCost);

  Route* addContact(Contact* contact);

  std::vector<Contact*> getRoute();

  std::vector<Contact*> getPrefixRoute(uint p);

  double getRouteCost();

  __uint128_t getHash(uint s, uint e) {
    return (((incrementalH[e] - incrementalH[s] + MOD) % MOD) * incrementalPI[s]) % MOD;
  }

  void debug() {
    for (Contact* contact : route) {
      contact->debug();
    }
    std::cout << "Route cost: " << routeCost << std::endl;
  }
};

#endif