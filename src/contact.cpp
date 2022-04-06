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

bool Contact::canGoTo(Contact* contact, double time) {
  return double_less(time, contact->getEnd());
}

double Contact::edgeCost(Contact* contact, double time) {
  if (!canGoTo(contact, time)) {
    std::cerr << "You can't go to that contact" << std::endl;
    exit(-1);
  }
  if (time < contact->getStart()) {
    return contact->getStart() + contact->getOwlt();
  }

  return time + contact->getOwlt();
}