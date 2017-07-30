#ifndef UTIL_H
#define UTIL_H

#include <Rcpp.h>

#include "def.h"

template<typename T>
T sum(const std::vector<T>& v) {
  return std::accumulate(v.cbegin(), v.cend(), (T) 0,
                         [](const T acc,
                            const T val) {
                           return acc + val;
                         });
}

template<typename T>
T square_sum(const std::vector<T>& v) {
  return std::accumulate(v.cbegin(), v.cend(), (T) 0,
                         [](const T acc,
                            const T val) {
                           return acc + val * val;
                         });
}

template<typename T>
double mean(const std::vector<T>& v) {
  int n;
  double mean = 0.0;

  for (auto x : v) {
    mean += (x - mean) / ++n;
  }

  return mean;
}

template<typename T>
double sd(const std::vector<T>& v, bool corrected=true) {
  double avg = mean(v);
  return std::accumulate(v.cbegin(), v.cend(), (double) 0.0,
                         [avg](double acc, double val) {
                           return acc + pow(val - avg, 2);
                         }) / (v.size() - corrected);
}

inline std::string to_lower(const std::string& s) {
  std::string l{s};
  std::transform(s.cbegin(), s.cend(), l.begin(), ::tolower);
  return l;
}

inline Corpus convert_from_R(const Rcpp::List& corpus) {
  Corpus c;

  for (auto const& document : corpus) {
    c.push_back(Rcpp::as<Document>(document));
  }

  return c;
}

inline Document convert_from_R(const Rcpp::StringVector& document) {
  return Rcpp::as<Document>(document);
}

inline Rcpp::StringVector convert_to_R(const DocumentSegments& segments) {
  return Rcpp::wrap(segments);
}

inline Rcpp::List convert_to_R(const CorpusSegments& segments) {
  return Rcpp::wrap(segments);
}

#endif // UTIL_H
