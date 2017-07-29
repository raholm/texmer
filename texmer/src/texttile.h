#ifndef TEXTTILE_H
#define TEXTTILE_H

#include <Rcpp.h>

#include "def.h"
#include "token_sequence.h"

class TextTile {
public:
  TextTile(std::size_t sentence_size, std::size_t block_size, const std::string method, bool liberal);

  ~TextTile() = default;

  std::vector<BoundaryPoints> get_boundary_points(const Corpus& corpus, const Stopwords& stopwords);

private:
  std::size_t sentence_size;
  std::size_t block_size;

  std::string method;
  bool liberal;

  // Token Sequences
  CorpusTokenSequences create_token_sequences_from_corpus(const Corpus& tokens,
                                                          std::size_t sentence_size,
                                                          const Stopwords& stopwords);

  DocTokenSequences create_token_sequences_from_doc(const Doc& tokens,
                                                    std::size_t sentence_size,
                                                    const Vocabulary& stopwords);

  // Block Scores
  CorpusScores calculate_block_scores_for_corpus(const CorpusTokenSequences& token_sequences,
                                                 std::size_t block_size);
  DocScores calculate_block_scores_for_doc(const DocTokenSequences& token_sequences,
                                           std::size_t block_size);
  TokenSequence create_token_sequence_from_block(const BlockTokenSequences& block);
  Score bs_calculate_score(const TokenSequence& left, const TokenSequence& right);
  std::size_t bs_calculate_numerator(const TokenSequence& left, const TokenSequence& right);
  double bs_calculate_denominator(const TokenSequence& left, const TokenSequence& right);

  // Vocabulary Scores
  CorpusScores calculate_vocabulary_scores_for_corpus(const CorpusTokenSequences& token_sequences,
                                                      std::size_t sentence_size);
  DocScores calculate_vocabulary_scores_for_doc(const DocTokenSequences& token_sequences,
                                                std::size_t sentence_size);


  // Boundary Points
  BoundaryPoints find_gap_boundaries_from_doc_scores(const DocScores& lexical_scores,
                                                     bool liberal);
  double get_depth_cutoff_score(const DocScores& lexical_scores,
                                bool liberal);
  double get_depth_score_by_side(const DocScores& lexical_scores,
                                 unsigned gap,
                                 bool left);

};


CorpusTokenSequences create_token_sequences_from_corpus(const RCorpus& tokens,
                                                        std::size_t sentence_size,
                                                        const RStopwords& stopwords);

CorpusTokenSequences create_token_sequences_from_corpus(const Corpus& tokens,
                                                        std::size_t sentence_size,
                                                        const Stopwords& stopwords);

DocTokenSequences create_token_sequences_from_doc(const Doc& tokens,
                                                  std::size_t sentence_size,
                                                  const Vocabulary& stopwords);

// Calculating block scores
CorpusScores calculate_block_scores_for_corpus(const CorpusTokenSequences& token_sequences,
                                               std::size_t block_size);
DocScores calculate_block_scores_for_doc(const DocTokenSequences& token_sequences,
                                         std::size_t block_size);
TokenSequence create_token_sequence_from_block(const BlockTokenSequences& block);
Score bs_calculate_score(const TokenSequence& left, const TokenSequence& right);
std::size_t bs_calculate_numerator(const TokenSequence& left, const TokenSequence& right);
double bs_calculate_denominator(const TokenSequence& left, const TokenSequence& right);

// Calculating vocabulary scores
CorpusScores calculate_vocabulary_scores_for_corpus(const CorpusTokenSequences& token_sequences,
                                                    std::size_t sentence_size);
DocScores calculate_vocabulary_scores_for_doc(const DocTokenSequences& token_sequences,
                                              std::size_t sentence_size);

// Finding gap boundaries
BoundaryPoints find_gap_boundaries_from_doc_scores(const DocScores& lexical_scores,
                                                   bool liberal);
double get_depth_cutoff_score(const DocScores& lexical_scores,
                              bool liberal);
double get_depth_score_by_side(const DocScores& lexical_scores,
                               unsigned gap,
                               bool left);

Rcpp::IntegerVector generate_texttile_segment_ids_from_boundry_points_cpp(Rcpp::IntegerVector& boundary_pts,
                                                                          unsigned n);
void fill_ids(Rcpp::IntegerVector* ids, unsigned* from, int with, int times);

#endif // TEXTTILE_H
