#ifndef UTIL_H
#define UTIL_H

#include "def.h"

#include <numeric>
#include <algorithm>
#include <cmath>

template<typename T>
T sum(const std::vector<T>& v) {
  return std::accumulate(v.cbegin(), v.cend(), 0.0,
                         [](const T acc,
                            const T val) {
                           return acc + val;
                         });
}

template<typename T>
T square_sum(const std::vector<T>& v) {
  return std::inner_product(v.cbegin(), v.cend(), v.cbegin(), 0.0);
}

template<typename T>
double mean(const std::vector<T>& v) {
  int n = 0;
  double mean = 0.0;

  for (auto const& x : v) {
    mean += (x - mean) / ++n;
  }

  return mean;
}

template<typename T>
double sd(const std::vector<T>& v, bool corrected=true) {
  double avg = mean(v);
  return sqrt(std::accumulate(v.cbegin(), v.cend(), 0.0,
                              [avg](double acc, T val) {
                                return acc + pow(val - avg, 2);
                              }) / (v.size() - corrected));
}

inline std::string to_lower(const std::string& s) {
  std::string l{s};
  std::transform(s.cbegin(), s.cend(), l.begin(), ::tolower);
  return l;
}

#endif // UTIL_H
