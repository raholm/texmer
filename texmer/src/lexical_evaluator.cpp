#include "lexical_evaluator.h"

#include <exception>

#include "util.h"

BlockEvaluator::BlockEvaluator(std::size_t block_size)
  : block_size_(block_size)
{
  if (block_size_ < 1) {
    throw std::invalid_argument("Invalid block size: '" + std::to_string(block_size_) + "'.");
  }
}

CorpusScores BlockEvaluator::evaluate(const CorpusTokenSequences& token_sequences) const {
  std::size_t n = token_sequences.size();
  CorpusScores scores(n);

  for (unsigned i = 0; i < n; ++i) {
    scores.at(i) = evaluate(token_sequences.at(i));
  }

  return scores;
}

DocumentScores BlockEvaluator::evaluate(const DocumentTokenSequences& token_sequences) const {
  std::size_t n = token_sequences.size();

  if (n == 0 || n == 1)
    return DocumentScores{0};

  DocumentScores scores(n, 0);

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

    left_block = BlockTokenSequences(token_sequences.cbegin() + left_start_offset,
                                     token_sequences.cbegin() + left_end_offset);

    right_block = BlockTokenSequences(token_sequences.cbegin() + right_start_offset,
                                      token_sequences.cbegin() + right_end_offset);

    scores.at(gap) = compute_score(left_block, right_block);
  }

  return scores;
}

TokenSequence BlockEvaluator::create_token_sequence(const BlockTokenSequences& block) const {
  TokenSequence result;

  for (auto const& ts : block) {
    result += ts;
  }

  return result;
}


Score BlockEvaluator::compute_score(const BlockTokenSequences& left,
                                    const BlockTokenSequences& right) const {
  TokenSequence left_ts = create_token_sequence(left);
  TokenSequence right_ts = create_token_sequence(right);

  std::size_t numerator = compute_numerator(left_ts, right_ts);
  double denominator = compute_denominator(left_ts, right_ts);

  return numerator / denominator;
}

std::size_t BlockEvaluator::compute_numerator(const TokenSequence& left,
                                              const TokenSequence& right) const {
  TokenSequence ts = left * right;
  return sum(ts.get_values());
}

double BlockEvaluator::compute_denominator(const TokenSequence& left,
                                           const TokenSequence& right) const {
  std::vector<std::size_t> left_counts = left.get_values();
  std::vector<std::size_t> right_counts = right.get_values();

  std::size_t left_sq_sum = square_sum(left_counts);
  std::size_t right_sq_sum = square_sum(right_counts);

  return sqrt(left_sq_sum * right_sq_sum);
}

VocabularyEvaluator::VocabularyEvaluator(std::size_t sentence_size)
  : sentence_size_(sentence_size)
{
  if (sentence_size_ < 1) {
    throw std::invalid_argument("Invalid sentence size: '" + std::to_string(sentence_size_) + "'.");
  }

}

CorpusScores VocabularyEvaluator::evaluate(const CorpusTokenSequences& token_sequences) const {
  std::size_t n = token_sequences.size();
  CorpusScores scores(n);

  for (unsigned i = 0; i < n; ++i) {
    scores.at(i) = evaluate(token_sequences.at(i));
  }

  return scores;
}

DocumentScores VocabularyEvaluator::evaluate(const DocumentTokenSequences& token_sequences) const {
  std::size_t n = token_sequences.size();

  if (n == 0 || n == 1)
    return DocumentScores{0};

  DocumentScores scores(n, 0);
  double denom = 2 * sentence_size_;
  std::size_t n_gaps = n - 1;

  std::vector<Vocabulary> vocabularies = get_vocabularies(token_sequences);

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

std::vector<Vocabulary> VocabularyEvaluator::get_vocabularies(const DocumentTokenSequences& token_sequences) const {
  std::size_t n = token_sequences.size();
  std::vector<Vocabulary> vocabularies(n);

  for (unsigned i = 0; i < n; ++i) {
    vocabularies.at(i) = token_sequences.at(i).get_vocabulary();
  }

  return vocabularies;
}
