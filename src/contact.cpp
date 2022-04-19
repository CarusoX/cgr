#include "contact.hpp"

Contact::Contact(std::string _from, std::string _to, uint _start, uint _end, double _rate, double _owlt) {
  from = _from;
  to = _to;
  start = _start;
  end = _end;
  rate = _rate;
  owlt = _owlt;
}

std::string Contact::getFrom() {
  return from;
}

std::string Contact::getTo() {
  return to;
}

uint Contact::getStart() {
  return start;
}

uint Contact::getEnd() {
  return end;
}

double Contact::getRate() {
  return rate;
}

double Contact::getOwlt() {
  return owlt;
}

bool Contact::canGoTo(double time) {
  return double_less(time, this->getEnd());
}

double Contact::edgeCost(double time) {
  if (!canGoTo(time)) {
    std::cerr << "You can't use this contact at time " << time << std::endl;
    exit(-1);
  }
  if (double_less(time, this->getStart())) {
    return this->getStart() + this->getOwlt();
  }

  return time + this->getOwlt();
}