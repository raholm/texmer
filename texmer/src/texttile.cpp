#include "texttile.h"

#include <algorithm>
#include <cmath>

#include "util.h"

CorpusTokenSequences create_token_sequences_from_corpus(const RCorpus& corpus,
                                                        std::size_t sentence_size,
                                                        const RStopwords& stopwords) {
  CorpusTokenSequences token_sequences;
  Doc doc;
  Vocabulary stopwords_voc(stopwords);

  for (unsigned i = 0; i < corpus.size(); ++i) {
    doc = Rcpp::as<Doc>(corpus[i]);
    token_sequences.push_back(create_token_sequences_from_doc(doc,
                                                              sentence_size,
                                                              stopwords_voc));
  }

  return token_sequences;
}

CorpusTokenSequences create_token_sequences_from_corpus(const Corpus& corpus,
                                                        std::size_t sentence_size,
                                                        const Stopwords& stopwords) {
  CorpusTokenSequences token_sequences;
  Doc doc;
  Vocabulary stopwords_voc(stopwords);

  for (unsigned i = 0; i < corpus.size(); ++i) {
    doc = corpus[i];
    token_sequences.push_back(create_token_sequences_from_doc(doc,
                                                              sentence_size,
                                                              stopwords_voc));
  }

  return token_sequences;
}

DocTokenSequences create_token_sequences_from_doc(const Doc& tokens,
                                                  std::size_t sentence_size,
                                                  const Vocabulary& stopwords) {
  DocTokenSequences token_sequences;
  std::size_t nseg = ceil(tokens.size() / sentence_size);

  std::size_t start, end;
  std::vector<std::string> segment;
  TokenSequence token_sequence;

  for (unsigned i = 0; i < nseg; ++i) {
    start = i * sentence_size;
    end = std::max(start + sentence_size, tokens.size());

    segment = Doc(tokens.begin() + start,
                  tokens.begin() + end);
    token_sequence = TokenSequence(segment);
    token_sequence -= stopwords;
    token_sequences.push_back(token_sequence);
  }

  return token_sequences;
}

CorpusScores calculate_block_scores_for_corpus(const CorpusTokenSequences& token_sequences,
                                               std::size_t block_size) {
  CorpusScores scores;

  for (auto const& token_sequence : token_sequences) {
    scores.push_back(calculate_block_scores_for_doc(token_sequence,
                                                    block_size));
  }

  return scores;
}

DocScores calculate_block_scores_for_doc(const DocTokenSequences& token_sequences,
                                         std::size_t block_size) {
  std::size_t n = token_sequences.size();
  DocScores scores(n, 0.0);

  BlockTokenSequences left_block, right_block;
  TokenSequence left_block_ts, right_block_ts;

  std::size_t current_block_size;
  std::size_t left_start_offset, left_end_offset;
  std::size_t right_start_offset, right_end_offset;

  for (std::size_t gap = 0; gap < (n - 1); ++gap) {
    current_block_size = std::min({gap, block_size, n - gap});

    left_start_offset = gap - current_block_size;
    left_end_offset = gap + 1;

    right_start_offset = left_end_offset;
    right_end_offset = right_start_offset + current_block_size + 1;

    left_block = BlockTokenSequences(token_sequences.cbegin() + left_start_offset,
                                     token_sequences.cbegin() + left_end_offset);

    right_block = BlockTokenSequences(token_sequences.cbegin() + right_start_offset,
                                      token_sequences.cbegin() + right_end_offset);

    left_block_ts = create_token_sequence_from_block(left_block);
    right_block_ts = create_token_sequence_from_block(right_block);


    scores[gap] = bs_calculate_score(left_block_ts, right_block_ts);
  }

  return scores;
}


TokenSequence create_token_sequence_from_block(const BlockTokenSequences& block) {
  TokenSequence result;
  for (auto const& ts : block) {
    result += ts;
  }
  return result;
}

Score bs_calculate_score(const TokenSequence& left, const TokenSequence& right) {
  std::size_t numerator = bs_calculate_numerator(left, right);
  double denominator = bs_calculate_denominator(left, right);
  return numerator / denominator;
}

std::size_t bs_calculate_numerator(const TokenSequence& left, const TokenSequence& right) {
  TokenSequence ts = left * right;
  std::vector<std::size_t> counts = ts.get_counts();
  return sum(counts);
}

double bs_calculate_denominator(const TokenSequence& left, const TokenSequence& right) {
  std::vector<std::size_t> left_counts = left.get_counts();
  std::vector<std::size_t> right_counts = right.get_counts();

  std::size_t left_sq_sum = square_sum(left_counts);
  std::size_t right_sq_sum = square_sum(right_counts);

  return sqrt(left_sq_sum * right_sq_sum);
}

CorpusScores calculate_vocabulary_scores_for_corpus(const CorpusTokenSequences& token_sequences,
                                                    std::size_t sentence_size) {
  return;
}

DocScores calculate_vocabulary_scores_for_doc(const DocTokenSequences& token_sequences,
                                              std::size_t sentence_size) {
  return;
}

BoundaryPoints find_gap_boundaries_from_doc_scores(const DocScores& lexical_scores,
                                                   bool liberal) {
  BoundaryPoints boundaries;

  double cutoff_score = get_depth_cutoff_score(lexical_scores, liberal);
  double score, depth_score, left_depth_score, right_depth_score;

  for (unsigned gap = 0; gap < lexical_scores.size(); ++gap) {
    score = lexical_scores[gap];

    left_depth_score = get_depth_score_by_side(lexical_scores, gap, true);
    right_depth_score = get_depth_score_by_side(lexical_scores, gap, false);

    depth_score = left_depth_score + right_depth_score;

    if (depth_score >= cutoff_score)
      boundaries.push_back(gap + 1);
  }

  return boundaries;
}

double get_depth_cutoff_score(const DocScores& lexical_scores,
                              bool liberal) {
  double avg = mean(lexical_scores);
  double stdev = sd(lexical_scores);

  if (liberal) return avg - stdev;
  else return avg - stdev / 2;
}

double get_depth_score_by_side(const DocScores& lexical_scores,
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
