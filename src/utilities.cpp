#include "utilities.hpp"

bool double_equal(double a, double b, double eps) {
  return abs(a - b) <= eps;
}

bool double_less(double a, double b, double eps) {
  return a + eps < b;
}

bool path_with_cost_greater(const std::pair<double, path> s1, const std::pair<double, path> s2) {
  if(double_equal(s1.first, s2.first)) {
    if(s1.second.size() == s2.second.size()) {
      return s1.second > s2.second;
    }
    return s1.second.size() > s2.second.size();
  }
  return double_less(s2.first, s1.first);
}