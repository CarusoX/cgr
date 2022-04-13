#include "utilities.hpp"

bool double_equal(double a, double b, double eps) {
  return abs(a - b) <= eps;
}

bool double_less(double a, double b, double eps) {
  return a + eps < b;
}