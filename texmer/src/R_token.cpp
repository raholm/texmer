#include <Rcpp.h>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cmath>

Rcpp::IntegerVector get_repeat_counts(int n_tokens, int segsize);
void print(const Rcpp::IntegerVector& vec);

// [[Rcpp::export]]
Rcpp::IntegerVector get_token_segment_ids_cpp(const Rcpp::IntegerVector& n_tokens_per_doc,
                                              const Rcpp::IntegerVector& n_segs_per_doc,
                                              const Rcpp::IntegerVector& segsize_per_doc) {
  int n = Rcpp::sum(n_tokens_per_doc);

  Rcpp::IntegerVector ids(n), id_repeat_counts;

  int max_id = 1;
  int id_idx = 0;
  int current_id, n_tokens, n_segs, segsize;

  for (unsigned i = 0; i < n_tokens_per_doc.size(); ++i) {
    n_tokens = n_tokens_per_doc[i];
    n_segs = n_segs_per_doc[i];
    segsize = segsize_per_doc[i];

    id_repeat_counts = get_repeat_counts(n_tokens, segsize);

    for (unsigned seg_id = 0; seg_id < n_segs; ++seg_id) {
      current_id = seg_id + max_id;

      for (unsigned j = 0; j < id_repeat_counts[seg_id]; ++j) {
        ids[id_idx++] = current_id;
      }
    }

    max_id = current_id + 1;
  }

  return ids;
}

Rcpp::IntegerVector get_repeat_counts(int n_tokens, int segsize) {
  int n = ceil((double) n_tokens / segsize);

  Rcpp::IntegerVector counts(n);

  int left_count = 0;
  int right_count = segsize;

  for (unsigned i = 0; i < (n - 1); ++i) {
    counts[i] = right_count - left_count;

    left_count += segsize;
    right_count += segsize;
  }

  counts[n - 1] = n_tokens - left_count;

  return counts;
}

void print(const Rcpp::IntegerVector& vec) {
  for (unsigned i = 0; i < vec.size(); ++i) {
    Rcpp::Rcout << vec[i] << " ";
  }
}
