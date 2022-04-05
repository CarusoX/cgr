#include "utilities.hpp"

bool double_equal(double a, double b, double eps) {
  return abs(a - b) <= EPS;
}

bool double_less(double a, double b, double eps) {
  return a + EPS < b;
}