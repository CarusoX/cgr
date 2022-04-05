#ifndef UTILITIES_H
#define UTILITIES_H

#include <algorithm>

const double EPS = 1e-9;

bool double_equal(double a, double b, double eps = EPS);

bool double_less(double a, double b, double eps = EPS);

#endif