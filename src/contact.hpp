#ifndef CONTACT_H
#define CONTACT_H

#include <iostream>
#include <string>
#include "node.hpp"
#include "utilities.hpp"

class Contact : Node {

private:
  std::string from; // from which node
  std::string to; // to which node
  uint start; // start of the communication window
  uint end; // end of the communication window
  double rate; // rate of the communication
  double owlt; // owlt of the communication

public:
  Contact(std::string _from, std::string _to, uint start, uint end, double rate, double owlt);

  std::string getFrom() const;

  std::string getTo() const;

  uint getStart() const;

  uint getEnd() const;

  double getRate() const;

  double getOwlt() const;

  bool canGoTo(double time) const;

  double edgeCost(double time) const;

  void debug() {
    std::cout << from << " " << to << " " << start << " " << end << " " << rate << " " << owlt << "\n";
  }
};

using ContactT = Contact*;

#endif
