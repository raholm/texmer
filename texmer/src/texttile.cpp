#include "texttile.h"

#include <algorithm>
#include <cmath>

std::vector<std::vector<TokenSequence>> create_token_sequences(const Rcpp::List& doc_tokens,
                                                               std::size_t sentence_size,
                                                               const Rcpp::StringVector& stopwords) {
  return;
}

Rcpp::NumericVector calculate_block_scores(const std::vector<std::vector<TokenSequence>>& token_sequences,
                                           std::size_t block_size) {
  return;
}


TokenSequence create_token_sequence_from_block(const std::vector<TokenSequence>& block) {
  TokenSequence result;
  for (auto const& ts : block) {
    result += ts;
  }
  return result;
}

std::size_t bs_calculate_numerator(const TokenSequence& left, const TokenSequence& right) {
  TokenSequence ts = left * right;
  std::vector<std::size_t> counts = ts.get_counts();
  return std::accumulate(counts.cbegin(), counts.cend(), 0,
                         [](const std::size_t acc,
                            const std::size_t count) {
                           return acc + count;
                         });
}

double bs_calculate_denominator(const TokenSequence& left, const TokenSequence& right) {
  std::vector<std::size_t> left_counts = left.get_counts();
  std::vector<std::size_t> right_counts = right.get_counts();

  std::size_t left_sq_sum = bs_compute_square_sum(left_counts);
  std::size_t right_sq_sum = bs_compute_square_sum(right_counts);

  return sqrt(left_sq_sum * right_sq_sum);
}

std::size_t bs_compute_sum(const std::vector<std::size_t>& v) {
  return std::accumulate(v.cbegin(), v.cend(), 0,
                         [](const std::size_t acc,
                            const std::size_t val) {
                           return acc + val;
                         });
}

std::size_t bs_compute_square_sum(const std::vector<std::size_t>& v) {
  return std::accumulate(v.cbegin(), v.cend(), 0,
                         [](const std::size_t acc,
                            const std::size_t val) {
                           return acc + val * val;
                  });
}

Rcpp::NumericVector calculate_vocabulary_scores(const std::vector<std::vector<TokenSequence>>& token_sequences,
                                                std::size_t sentence_size) {
  return;
}


// [[Rcpp::export]]
Rcpp::IntegerVector find_gap_boundaries_cpp(Rcpp::NumericVector& lexical_scores,
                                            bool liberal) {
  Rcpp::IntegerVector boundaries;

  double cutoff_score = get_depth_cutoff_score(lexical_scores, liberal);
  double score, depth_score, left_depth_score, right_depth_score;

  for (unsigned i = 0; i < lexical_scores.size(); ++i) {
    score = lexical_scores[i];

    left_depth_score = get_depth_score_by_side(lexical_scores, i, true);
    right_depth_score = get_depth_score_by_side(lexical_scores, i, false);

    depth_score = left_depth_score + right_depth_score;

    if (depth_score >= cutoff_score)
      boundaries.push_back(i + 1);
  }

  return boundaries;
}

double get_depth_cutoff_score(Rcpp::NumericVector& lexical_scores,
                              bool liberal) {
  double avg = Rcpp::mean(lexical_scores);
  double stdev = Rcpp::sd(lexical_scores);

  if (liberal) return avg - stdev;
  else return avg - stdev / 2;
}

double get_depth_score_by_side(Rcpp::NumericVector& lexical_scores,
                               unsigned gap,
                               bool left) {
  double depth_score = 0;
  unsigned current_gap = gap;

  while ((lexical_scores[current_gap] - lexical_scores[gap] >= depth_score)) {
    depth_score = lexical_scores[current_gap] - lexical_scores[gap];

    if (left) --current_gap;
    else ++current_gap;

    if (current_gap < 0 || current_gap >= lexical_scores.size())
      break;
  }

  return depth_score;
}

// [[Rcpp::export]]
Rcpp::IntegerVector generate_texttile_segment_ids_from_boundry_points_cpp(Rcpp::IntegerVector& boundary_pts,
                                                                          unsigned n) {
  Rcpp::IntegerVector ids(n);

  unsigned nseg = boundary_pts.size() + 1;
  unsigned id_idx = 0;

  for (unsigned seg_id = 0; seg_id < nseg; ++seg_id) {
    if (seg_id == 0) {
      fill_ids(&ids, &id_idx, seg_id + 1, boundary_pts[seg_id]);
    } else if (seg_id == (nseg - 1)) {
      fill_ids(&ids, &id_idx, seg_id + 1, n - boundary_pts[seg_id - 1]);
    } else {
      fill_ids(&ids, &id_idx, seg_id + 1, boundary_pts[seg_id] - boundary_pts[seg_id - 1]);
    }
  }

  return ids;
}

void fill_ids(Rcpp::IntegerVector* ids, unsigned* from, int with, int times) {
  Rcpp::IntegerVector vec = *ids;
  unsigned idx = *from;
  for (unsigned i = 0; i < times; ++i) {
    vec[idx++] = with;
  }
  *from = idx;
}
