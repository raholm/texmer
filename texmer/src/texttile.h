#ifndef TEXTTILE_H
#define TEXTTILE_H

#include <Rcpp.h>

#include "token_sequence.h"

std::vector<std::vector<TokenSequence>> create_token_sequences(const Rcpp::List& doc_tokens,
                                                               std::size_t sentence_size,
                                                               const Rcpp::StringVector& stopwords);

// Calculating block scores
Rcpp::NumericVector calculate_block_scores(const std::vector<std::vector<TokenSequence>>& token_sequences,
                            std::size_t block_size);
TokenSequence create_token_sequence_from_block(const std::vector<TokenSequence>& block);
std::size_t bs_calculate_numerator(const TokenSequence& left, const TokenSequence& right);
double bs_calculate_denominator(const TokenSequence& left, const TokenSequence& right);
std::size_t bs_compute_sum(const std::vector<std::size_t>& v);
std::size_t bs_compute_square_sum(const std::vector<std::size_t>& v);

// Calculating vocabulary scores
Rcpp::NumericVector calculate_vocabulary_scores(const std::vector<std::vector<TokenSequence>>& token_sequences,
                                 std::size_t sentence_size);

// Finding gap boundaries
Rcpp::IntegerVector find_gap_boundaries_cpp(Rcpp::NumericVector& lexical_scores,
                                            bool liberal);
double get_depth_cutoff_score(Rcpp::NumericVector& lexical_scores,
                              bool liberal);
double get_depth_score_by_side(Rcpp::NumericVector& lexical_scores,
                               unsigned gap,
                               bool left);

Rcpp::IntegerVector generate_texttile_segment_ids_from_boundry_points_cpp(Rcpp::IntegerVector& boundary_pts,
                                                                          unsigned n);
void fill_ids(Rcpp::IntegerVector* ids, unsigned* from, int with, int times);

#endif // TEXTTILE_H
