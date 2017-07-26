#include "token.h"

#include <iostream>
#include <algorithm>
#include <iterator>
#include <cmath>

// [[Rcpp::export]]
Rcpp::IntegerVector generate_token_segment_ids_cpp(const Rcpp::IntegerVector& ntoken_per_doc,
                                                   const Rcpp::IntegerVector& nseg_per_doc,
                                                   const Rcpp::IntegerVector& segsize_per_doc) {
  int n = Rcpp::sum(ntoken_per_doc);

  Rcpp::IntegerVector ids(n);
  Rcpp::IntegerVector id_repeat_counts;

  int max_id = 1;
  int id_idx = 0;
  int current_id;

  int ntoken, nseg, segsize;

  for (unsigned i = 0; i < ntoken_per_doc.size(); ++i) {
    ntoken = ntoken_per_doc[i];
    nseg = nseg_per_doc[i];
    segsize = segsize_per_doc[i];

    id_repeat_counts = generate_repeat_count_vec(ntoken, segsize);

    for (unsigned seg_id = 0; seg_id < nseg; ++seg_id) {
      current_id = seg_id + max_id;

      for (unsigned j = 0; j < id_repeat_counts[seg_id]; ++j) {
        ids[id_idx++] = current_id;
      }
    }

    max_id = current_id + 1;
  }

  return ids;
}

Rcpp::IntegerVector generate_repeat_count_vec(int ntoken, int segsize) {
  int n = ceil((double) ntoken / segsize);

  Rcpp::IntegerVector counts(n);

  int left_count = 0;
  int right_count = segsize;

  for (unsigned i = 0; i < (n - 1); ++i) {
    counts[i] = right_count - left_count;

    left_count += segsize;
    right_count += segsize;
  }

  counts[n - 1] = ntoken - left_count;

  return counts;
}

void print_vec(const Rcpp::IntegerVector& vec) {
  for (unsigned i = 0; i < vec.size(); ++i) {
    std::cout << vec[i] << " ";
  }
  std::cout << std::endl;
}
