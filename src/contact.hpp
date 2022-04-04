#ifndef CONTACT_H
#define CONTACT_H

#include <iostream>
#include <string>

class Contact {

private:
  std::string from; // from which node
  std::string to; // to which node
  uint start; // start of the communication window
  uint end; // end of the communication window
  double rate; // rate of the communication
  double owlt; // owlt of the communication

public:
  Contact(std::string _from, std::string _to, uint start, uint end, double rate, double owlt);

  std::string getFrom();

  std::string getTo();

  uint getStart();

  uint getEnd();

  double getRate();

  double getOwlt();

  void debug() {
    std::cout << from << " " << to << " " << start << " " << end << " " << rate << " " << owlt << "\n";
  }
};

#endif