#include "read.hpp"


std::vector<std::string> split_by(std::string str, std::string delimiter) {
  size_t last = 0;
  size_t next = 0;
  std::vector<std::string> parts;
  while ((next = str.find(delimiter, last)) != std::string::npos) {
    parts.push_back(str.substr(last, next - last));
    last = next + 1;
  }
  parts.push_back(str.substr(last, next - last));
  return parts;
}

std::vector<ContactT> read_contact_plan(std::string path) {
  freopen(path.c_str(), "r", stdin);

  std::vector<ContactT> contacts;
  std::string line;

  // We read until we get EOL
  while (getline(std::cin, line)) {
    // We split the line by spaces
    std::vector<std::string> parts = split_by(line, " ");

    // We should have [from, to, start, end, rate, owlt]
    if (parts.size() != 6) {
      std::cerr << "Line " << line << " is not well formatted.\n";
      exit(-1);
    }

    std::string from = parts[0];
    std::string to = parts[1];
    uint start = atoi(parts[2].c_str());
    uint end = atoi(parts[3].c_str());
    double rate = atof(parts[4].c_str());
    double owlt = atof(parts[5].c_str());

    if (from == to) {
      std::cerr << "Don't specify the identity edges.\n";
      exit(-1);
    }

    contacts.push_back(new Contact(from, to, start, end, rate, owlt));
  }

  return contacts;
}