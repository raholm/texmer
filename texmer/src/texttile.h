#ifndef TEXTTILE_H
#define TEXTTILE_H

#include <Rcpp.h>

#include "token_sequence.h"

using Doc = std::vector<std::string>;
using RDoc = Rcpp::StringVector;

using Corpus = std::vector<Doc>;
using RCorpus = Rcpp::List;

using Stopwords = Doc;
using RStopwords = RDoc;

using CorpusTokenSequences = std::vector<std::vector<TokenSequence>>;
using DocTokenSequences = std::vector<TokenSequence>;

using TokenSequenceBlock = DocTokenSequences;

using Scores = std::vector<double>;


CorpusTokenSequences create_token_sequences(const RCorpus& tokens,
                                            std::size_t sentence_size,
                                            const RStopwords& stopwords);

CorpusTokenSequences create_token_sequences(const Corpus& tokens,
                                            std::size_t sentence_size,
                                            const Stopwords& stopwords);

DocTokenSequences create_token_sequences_for_doc(const Doc& tokens,
                                                 std::size_t sentence_size,
                                                 const Vocabulary& stopwords);

// Calculating block scores
Scores calculate_block_scores(const CorpusTokenSequences& token_sequences,
                                           std::size_t block_size);
TokenSequence create_token_sequence_from_block(const TokenSequenceBlock& block);
std::size_t bs_calculate_numerator(const TokenSequence& left, const TokenSequence& right);
double bs_calculate_denominator(const TokenSequence& left, const TokenSequence& right);
std::size_t bs_compute_sum(const std::vector<std::size_t>& v);
std::size_t bs_compute_square_sum(const std::vector<std::size_t>& v);

// Calculating vocabulary scores
Scores calculate_vocabulary_scores(const CorpusTokenSequences& token_sequences,
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
