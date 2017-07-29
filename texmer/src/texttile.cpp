#include "texttile.h"

#include <algorithm>
#include <cmath>
#include <exception>

#include "util.h"

TextTile::TextTile(std::size_t sentence_size, std::size_t block_size, const std::string& method, bool liberal)
  : sentence_size_(sentence_size), block_size_(block_size), method_(std::move(to_lower(method))), liberal_(liberal)
{
  if (sentence_size_ < 1) {
    throw std::invalid_argument("Invalid sentence size: '" + std::to_string(sentence_size) + "'.");
  }

  if (block_size_ < 1) {
    throw std::invalid_argument("Invalid block size: '" + std::to_string(block_size) + "'.");
  }

  if (method_ != "block" && method_ != "vocabulary") {
    throw std::invalid_argument("Invalid choice of method: '" + method + "'.");
  }
}

CorpusSegments TextTile::get_segments(const Corpus& corpus, const Stopwords& stopwords) {
  Vocabulary voc_stopwords(stopwords);
  std::vector<std::size_t> n_tokens(corpus.size());

  for (unsigned i = 0; i < corpus.size(); ++i) {
    n_tokens[i] = corpus[i].size();
  }

  auto corpus_ts = ts_create(corpus, voc_stopwords);

  // Rcpp::Rcout << "Token Sequences:" << std::endl;
  // for (auto const& doc_ts : corpus_ts) {
  //   Rcpp::Rcout << "Doc:" << std::endl;
  //   for (auto const& ts : doc_ts)
  //     ts.print(Rcpp::Rcout) << " ";
  //   Rcpp::Rcout << std::endl;
  // }
  // Rcpp::Rcout << std::endl;

  // Rcpp::Rcout << "Method: " << method_ << std::endl;

  auto scores = (method_ == "block") ? bs_calculate(corpus_ts) : vs_calculate(corpus_ts);

  // Rcpp::Rcout << "Scores:" << std::endl;
  // for (auto const& doc_scores : scores) {
  //   Rcpp::Rcout << "Doc:" << std::endl;
  //   for (auto const& score : doc_scores)
  //     Rcpp::Rcout << score << " ";
  //   Rcpp::Rcout << std::endl;
  // }

  auto boundaries = bp_find_boundaries(scores);

  // Rcpp::Rcout << "Boundaries:" << std::endl;
  // for (auto const& doc_boundaries : boundaries) {
  //   Rcpp::Rcout << "Doc:" << std::endl;
  //   for (auto const& bound : doc_boundaries)
  //     Rcpp::Rcout << bound << " ";
  //   Rcpp::Rcout << std::endl;
  // }

  auto segments = seg_generate(boundaries, n_tokens);

  // CorpusSegments segments;
  return segments;
}

DocSegments TextTile::get_segments(const Doc& doc, const Stopwords& stopwords) {
  Vocabulary voc_stopwords(stopwords);
  std::size_t n_tokens = doc.size();

  auto doc_ts = ts_create(doc, voc_stopwords);
  auto scores = (method_ == "block") ? bs_calculate(doc_ts) : vs_calculate(doc_ts);
  auto boundaries = bp_find_boundaries(scores);
  auto segments = seg_generate(boundaries, n_tokens);

  return segments;
}

CorpusTokenSequences TextTile::ts_create(const Corpus& corpus,
                                         const Vocabulary& stopwords) {
  CorpusTokenSequences token_sequences(corpus.size());
  Doc doc;

  for (unsigned i = 0; i < corpus.size(); ++i) {
    doc = corpus[i];
    token_sequences[i] = ts_create(doc, stopwords);
  }

  return token_sequences;
}

DocTokenSequences TextTile::ts_create(const Doc& tokens,
                                      const Vocabulary& stopwords) {
  std::size_t nseg = ceil(tokens.size() / sentence_size_);
  DocTokenSequences token_sequences;

  std::size_t start, end;
  Segment segment;
  TokenSequence token_sequence;

  for (unsigned i = 0; i < nseg; ++i) {
    start = i * sentence_size_;
    end = std::min(start + sentence_size_, tokens.size());

    segment = Segment(tokens.begin() + start, tokens.begin() + end);

    token_sequence = TokenSequence(segment);
    token_sequence -= stopwords;

    if (token_sequence.length() == 0)
      continue;

    token_sequences.push_back(token_sequence);
  }

  return token_sequences;
}

TokenSequence TextTile::ts_create(const BlockTokenSequences& block) {
  TokenSequence result;

  for (auto const& ts : block) {
    result += ts;
  }

  return result;
}

CorpusScores TextTile::bs_calculate(const CorpusTokenSequences& token_sequences) {
  // Rcpp::Rcout << "bs_calculate_corpus" << std::endl;;

  std::size_t n = token_sequences.size();
  CorpusScores scores(n);

  // Rcpp::Rcout << "corpus_ts Size: " << n << std::endl;

  for (unsigned i = 0; i < n; ++i) {
    scores[i] = bs_calculate(token_sequences[i]);
  }

  return scores;
}

DocScores TextTile::bs_calculate(const DocTokenSequences& token_sequences) {
  // Rcpp::Rcout << "bs_calculate_doc" << std::endl;

  std::size_t n = token_sequences.size();

  // Rcpp::Rcout << "doc_ts Size: " << n << std::endl;

  if (n == 0 || n == 1)
    return DocScores{0};

  DocScores scores(n, 0);

  BlockTokenSequences left_block, right_block;
  TokenSequence left_block_ts, right_block_ts;

  std::size_t current_block_size;
  std::size_t left_start_offset, left_end_offset;
  std::size_t right_start_offset, right_end_offset;
  std::size_t n_gaps = n - 1;
  std::size_t next_gap;

  for (std::size_t gap = 0; gap < n_gaps; ++gap) {
    next_gap = gap + 1;

    current_block_size = std::min({gap, block_size_, n_gaps - next_gap});

    left_start_offset = gap - current_block_size;
    left_end_offset = next_gap;

    right_start_offset = left_end_offset;
    right_end_offset = right_start_offset + current_block_size + 1;

    // Rcpp::Rcout << "current_block_size: " << current_block_size << ", "
    //             << "left_start: " << left_start_offset << ", "
    //             << "left_end: " << left_end_offset << ", "
    //             << "right_start: " << right_start_offset << ", "
    //             << "right_end: " << right_end_offset << std::endl;

    left_block = BlockTokenSequences(token_sequences.cbegin() + left_start_offset,
                                     token_sequences.cbegin() + left_end_offset);

    right_block = BlockTokenSequences(token_sequences.cbegin() + right_start_offset,
                                      token_sequences.cbegin() + right_end_offset);

    scores[gap] = bs_calculate(left_block, right_block);
    // Rcpp::Rcout << "Current score: " << scores[gap] << std::endl;
  }

  return scores;
}

Score TextTile::bs_calculate(const BlockTokenSequences& left, const BlockTokenSequences& right) {
  TokenSequence left_ts = ts_create(left);
  TokenSequence right_ts = ts_create(right);

  // Rcpp::Rcout << "Calculate score";
  // left_ts.print(Rcpp::Rcout) << std::endl;
  // right_ts.print(Rcpp::Rcout) << std::endl;

  std::size_t numerator = bs_calculate_numerator(left_ts, right_ts);
  double denominator = bs_calculate_denominator(left_ts, right_ts);
  return numerator / denominator;
}

std::size_t TextTile::bs_calculate_numerator(const TokenSequence& left, const TokenSequence& right) {
  TokenSequence ts = left * right;
  return sum(ts.get_counts());
}

double TextTile::bs_calculate_denominator(const TokenSequence& left, const TokenSequence& right) {
  std::vector<std::size_t> left_counts = left.get_counts();
  std::vector<std::size_t> right_counts = right.get_counts();

  std::size_t left_sq_sum = square_sum(left_counts);
  std::size_t right_sq_sum = square_sum(right_counts);

  return sqrt(left_sq_sum * right_sq_sum);
}

CorpusScores TextTile::vs_calculate(const CorpusTokenSequences& token_sequences) {
  std::size_t n = token_sequences.size();
  CorpusScores scores(n);

  for (unsigned i = 0; i < n; ++i) {
    scores[i] = vs_calculate(token_sequences[i]);
  }

  return scores;
}

DocScores TextTile::vs_calculate(const DocTokenSequences& token_sequences) {
  std::size_t n = token_sequences.size();

  if (n == 0 || n == 1)
    return DocScores{0};

  DocScores scores(n, 0);
  double denom = 2 * sentence_size_;
  std::size_t n_gaps = n - 1;

  std::vector<Vocabulary> vocabularies = vs_get_vocabularies(token_sequences);

  Vocabulary left_tokens_seen;
  Vocabulary right_tokens_seen = vocabularies.at(0);

  Vocabulary left_new_tokens, right_new_tokens;
  Vocabulary left_tokens, right_tokens;

  for (unsigned gap = 0; gap < n_gaps; ++gap) {
    left_tokens = vocabularies.at(gap);
    right_tokens = vocabularies.at(gap + 1);

    left_new_tokens = left_tokens - left_tokens_seen;
    right_new_tokens = right_tokens - right_tokens_seen;

    scores.at(gap) = (left_new_tokens.size() + right_new_tokens.size()) / denom;

    left_tokens_seen += left_tokens;
    right_tokens_seen += right_tokens;
  }

  // Last score where we only look at the last token-sequence
  right_new_tokens = vocabularies.at(n_gaps) - left_tokens_seen;
  scores.at(n_gaps) = right_new_tokens.size() / denom;

  return scores;
}

std::vector<Vocabulary> TextTile::vs_get_vocabularies(const DocTokenSequences& token_sequences) {
  std::size_t n = token_sequences.size();
  std::vector<Vocabulary> vocabularies(n);

  for (unsigned i = 0; i < n; ++i) {
    vocabularies.at(i) = token_sequences.at(i).get_vocabulary();
  }

  return vocabularies;
}

std::vector<BoundaryPoints> TextTile::bp_find_boundaries(const CorpusScores& lexical_scores) {
  std::vector<BoundaryPoints> boundaries;

  for (auto const& scores : lexical_scores) {
    boundaries.push_back(bp_find_boundaries(scores));
  }

  return boundaries;
}

BoundaryPoints TextTile::bp_find_boundaries(const DocScores& lexical_scores) {
  BoundaryPoints boundaries;

  double cutoff_score = bp_calculate_depth_cutoff_score(lexical_scores);
  double score, depth_score, left_depth_score, right_depth_score;

  for (unsigned gap = 0; gap < lexical_scores.size(); ++gap) {
    score = lexical_scores[gap];

    left_depth_score = bp_calculate_depth_score_by_side(lexical_scores, gap, true);
    right_depth_score = bp_calculate_depth_score_by_side(lexical_scores, gap, false);

    depth_score = left_depth_score + right_depth_score;

    if (depth_score >= cutoff_score)
      boundaries.push_back(gap + 1);
  }

  return boundaries;
}

double TextTile::bp_calculate_depth_cutoff_score(const DocScores& lexical_scores) {
  double avg = mean(lexical_scores);
  double stdev = sd(lexical_scores);

  if (liberal_) return avg - stdev;
  else return avg - stdev / 2;
}

double TextTile::bp_calculate_depth_score_by_side(const DocScores& lexical_scores,
                                                  unsigned gap,
                                                  bool left) {
  double depth_score = 0;
  unsigned current_gap = gap;

  while ((lexical_scores[current_gap] - lexical_scores[gap] >= depth_score)) {
    depth_score = lexical_scores[current_gap] - lexical_scores[gap];

    if (left) --current_gap;
    else ++current_gap;

    if (current_gap < 0 || current_gap >= lexical_scores.size()) break;
  }

  return depth_score;
}

CorpusSegments TextTile::seg_generate(const std::vector<BoundaryPoints>& boundaries,
                                      std::vector<std::size_t> n_tokens) {
  std::size_t n = boundaries.size();
  CorpusSegments segments(n);

  for (unsigned i = 0; i < n; ++i) {
    segments[i] = seg_generate(boundaries[i], n_tokens[i]);
  }

  return segments;
}

DocSegments TextTile::seg_generate(const BoundaryPoints& boundaries,
                                   std::size_t n_tokens) {
  if (boundaries.size() == 0)
    return DocSegments(n_tokens, 1);

  DocSegments segments(n_tokens);

  std::size_t nseg = boundaries.size() + 1;
  std::size_t id_idx = 0;

  for (unsigned seg_id = 0; seg_id < nseg; ++seg_id) {
    if (seg_id == 0) {
      seg_fill(segments, id_idx, seg_id + 1, boundaries[seg_id]);
    } else if (seg_id == (nseg - 1)) {
      seg_fill(segments, id_idx, seg_id + 1, n_tokens - boundaries[seg_id - 1]);
    } else {
      seg_fill(segments, id_idx, seg_id + 1, boundaries[seg_id] - boundaries[seg_id - 1]);
    }
  }

  return segments;
}

void TextTile::seg_fill(DocSegments& segments, std::size_t& from, int with, int times) {
  for (unsigned i = 0; i < times; ++i) {
    segments[from++] = with;
  }
}

// [[Rcpp::export]]
Rcpp::List get_texttile_segments_cpp(const Rcpp::List& tokens,
                                     const Rcpp::StringVector& stopwords,
                                     std::size_t sentence_size,
                                     std::size_t block_size,
                                     const Rcpp::CharacterVector& method,
                                     bool liberal) {
  auto texttile = TextTile(sentence_size, block_size,
                           Rcpp::as<std::string>(method),
                           liberal);
  auto segments = texttile.get_segments(convert_from_R(tokens),
                                        convert_from_R(stopwords));
  return convert_to_R(segments);
}
