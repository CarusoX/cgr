#ifndef READ_H
#define READ_H

#include <iostream>
#include <string>
#include <vector>
#include "contact.hpp"

std::vector<Contact*> read_contact_plan(std::string path);

#endif