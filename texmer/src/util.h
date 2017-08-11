#ifndef TEXMER_UTIL_H_
#define TEXMER_UTIL_H_

#include "def.h"

#include <numeric>
#include <algorithm>
#include <cmath>

namespace texmer {

  template<typename T>
  inline T sum(const Vector<T>& v) {
    return std::accumulate(v.cbegin(), v.cend(), 0.0,
                           [](const T acc,
                              const T val) {
                             return acc + val;
                           });
  }

  template<typename T>
  inline T square_sum(const Vector<T>& v) {
    return std::inner_product(v.cbegin(), v.cend(), v.cbegin(), 0.0);
  }

  template<typename T>
  inline double mean(const Vector<T>& v) {
    int n = 0;
    double mean = 0.0;

    for (auto const& x : v) {
      mean += (x - mean) / ++n;
    }

    return mean;
  }

  template<typename T>
  inline double sd(const Vector<T>& v, bool corrected=true) {
    double avg = mean(v);
    return sqrt(std::accumulate(v.cbegin(), v.cend(), 0.0,
                                [avg](double acc, T val) {
                                  return acc + pow(val - avg, 2);
                                }) / (v.size() - corrected));
  }

  inline String to_lower(const String& s) {
    String l{s};
    std::transform(s.cbegin(), s.cend(), l.begin(), ::tolower);
    return l;
  }

} // namespace texmer

#endif // TEXMER_UTIL_H_
