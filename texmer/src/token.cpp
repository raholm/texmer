#include "token.h"

#include <iostream>
#include <algorithm>
#include <iterator>
#include <cmath>

// [[Rcpp::export]]
Rcpp::IntegerVector generate_token_segment_ids(const Rcpp::IntegerVector& ntokens_per_doc,
                                               const Rcpp::IntegerVector& nsegments_per_doc,
                                               const Rcpp::IntegerVector& segment_size_per_doc) {
  int n = Rcpp::sum(ntokens_per_doc);

  Rcpp::IntegerVector ids(n);
  Rcpp::IntegerVector current_seg_ids;
  Rcpp::IntegerVector id_repeat_counts;

  int max_id = 1;
  int id_idx = 0;

  int ntokens, nsegs, segsize;

  for (unsigned i = 0; i < ntokens_per_doc.size(); ++i) {
    ntokens = ntokens_per_doc[i];
    nsegs = nsegments_per_doc[i];
    segsize = segment_size_per_doc[i];

    id_repeat_counts = generate_repeat_count_vec(ntokens, segsize);

    for (unsigned seg_id = 0; seg_id < nsegs; ++seg_id) {
      current_seg_ids = repeat_int(seg_id + max_id, id_repeat_counts[seg_id]);

      for (unsigned j = 0; j < current_seg_ids.size(); ++j) {
        ids[id_idx++] = current_seg_ids[j];
      }
    }

    max_id = Rcpp::max(current_seg_ids) + 1;
  }

  return ids;
}

Rcpp::IntegerVector generate_repeat_count_vec(int ntokens, int segsize) {
  int n = ceil((double) ntokens / segsize);

  Rcpp::IntegerVector counts(n);

  int left_count = 0;
  int right_count = segsize;

  for (unsigned i = 0; i < (n - 1); ++i) {
    counts[i] = right_count - left_count;

    left_count += segsize;
    right_count += segsize;
  }

  counts[n - 1] = ntokens - left_count;

  return counts;
}

Rcpp::IntegerVector repeat_int(int value, int n) {
  return Rcpp::IntegerVector(n, value);
}

void print_vec(const Rcpp::IntegerVector& vec) {
  for (unsigned i = 0; i < vec.size(); ++i) {
    std::cout << vec[i] << " ";
  }
  std::cout << std::endl;
}
