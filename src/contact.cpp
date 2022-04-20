#include "contact.hpp"

Contact::Contact(std::string _from, std::string _to, uint _start, uint _end, double _rate, double _owlt) {
  from = _from;
  to = _to;
  start = _start;
  end = _end;
  rate = _rate;
  owlt = _owlt;
}

std::string Contact::getFrom() const {
  return from;
}

std::string Contact::getTo() const {
  return to;
}

uint Contact::getStart() const {
  return start;
}

uint Contact::getEnd() const {
  return end;
}

double Contact::getRate() const {
  return rate;
}

double Contact::getOwlt() const {
  return owlt;
}

bool Contact::canGoTo(double time) const {
  return double_less(time, this->getEnd());
}

double Contact::edgeCost(double time) const {
  if (!canGoTo(time)) {
    std::cerr << "You can't use this contact at time " << time << std::endl;
    exit(-1);
  }
  if (double_less(time, this->getStart())) {
    return this->getStart() + this->getOwlt();
  }

  return time + this->getOwlt();
}