#ifndef UTILITIES_H
#define UTILITIES_H

#include <algorithm>
#include <vector>

typedef unsigned int uint;

const double EPS = 1e-9;

bool double_equal(double a, double b, double eps = EPS);

bool double_less(double a, double b, double eps = EPS);

using path = std::vector<std::pair<uint, uint>>;

bool path_with_cost_greater(const std::pair<std::pair<double, uint>, path> s1, const std::pair<std::pair<double, uint>, path> s2);

#endif