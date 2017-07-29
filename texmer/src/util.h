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

double mean(const std::vector<double>& v) {
  int n;
  double mean = 0.0;

  for (auto x : v) {
    mean += (x - mean) / ++n;
  }

  return mean;
}

double sd(const std::vector<double>& v, bool corrected=true) {
  double avg = mean(v);
  return std::accumulate(v.cbegin(), v.cend(), (double) 0.0,
                         [avg](double acc, double val) {
                           return acc + pow(val - avg, 2);
                         }) / (v.size() - corrected);
}

Corpus convert_corpus_from_R(const RCorpus& rcorpus) {
  Corpus corpus;

  for (auto const& doc : rcorpus) {
    corpus.push_back(Rcpp::as<Doc>(doc));
  }

  return corpus;
}

Stopwords convert_stopwords_from_R(const RStopwords& rstopwords) {
  return Rcpp::as<Stopwords>(rstopwords);
}


#endif // UTIL_H
