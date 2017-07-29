#ifndef TEXTTILE_H
#define TEXTTILE_H

#include <Rcpp.h>

#include "token_sequence.h"
#include "def.h"

class TextTile {
public:
  TextTile(std::size_t sentence_size, std::size_t block_size, const std::string& method, bool liberal);

  ~TextTile() = default;

  CorpusSegments get_segments(const Corpus& corpus, const Stopwords& stopwords);
  DocSegments get_segments(const Doc& doc, const Stopwords& stopwords);

private:
  std::size_t sentence_size_;
  std::size_t block_size_;

  std::string method_;
  bool liberal_;

  // Token Sequences
  CorpusTokenSequences ts_create(const Corpus& tokens, const Vocabulary& stopwords);
  DocTokenSequences ts_create(const Doc& tokens, const Vocabulary& stopwords);
  TokenSequence ts_create(const BlockTokenSequences& block);

  // Block Scores
  CorpusScores bs_calculate(const CorpusTokenSequences& token_sequences);
  DocScores bs_calculate(const DocTokenSequences& token_sequences);
  Score bs_calculate(const BlockTokenSequences& left, const BlockTokenSequences& right);
  std::size_t bs_calculate_numerator(const TokenSequence& left, const TokenSequence& right);
  double bs_calculate_denominator(const TokenSequence& left, const TokenSequence& right);

  // Vocabulary Scores
  CorpusScores vs_calculate(const CorpusTokenSequences& token_sequences);
  DocScores vs_calculate(const DocTokenSequences& token_sequences);

  // Boundary Points
  std::vector<BoundaryPoints> bp_find_boundaries(const CorpusScores& lexical_scores);
  BoundaryPoints bp_find_boundaries(const DocScores& lexical_scores);
  double bp_calculate_depth_cutoff_score(const DocScores& lexical_scores);
  double bp_calculate_depth_score_by_side(const DocScores& lexical_scores, unsigned gap, bool left);

  // Segments
  CorpusSegments seg_generate(const std::vector<BoundaryPoints>& boundaries, std::vector<std::size_t> n_tokens);
  DocSegments seg_generate(const BoundaryPoints& boundaries, std::size_t n_tokens);
  void seg_fill(DocSegments& segments, std::size_t& from, int with, int times);

};

Rcpp::List get_texttile_segments_cpp(const Rcpp::List& tokens,
                                     const Rcpp::StringVector& stopwords,
                                     std::size_t sentence_size,
                                     std::size_t block_size,
                                     const Rcpp::CharacterVector& method,
                                     bool liberal);

#endif // TEXTTILE_H
