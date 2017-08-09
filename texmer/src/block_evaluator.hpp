#ifndef BLOCK_EVALUATOR_HPP
#define BLOCK_EVALUATOR_HPP

#include "util.h"

template<class SEQUENCE_T, typename T>
BlockEvaluator<SEQUENCE_T, T>::BlockEvaluator(std::size_t block_size)
  : block_size_(block_size)
{
  if (block_size_ < 1) {
    throw std::invalid_argument("Invalid block size: '" + std::to_string(block_size_) + "'.");
  }
}

template<class SEQUENCE_T, typename T>
CorpusScores BlockEvaluator<SEQUENCE_T, T>::evaluate(const CorpusSequences<SEQUENCE_T>& sequences) const {
  std::size_t n = sequences.size();
  CorpusScores scores(n);

  for (unsigned i = 0; i < n; ++i) {
    scores.at(i) = evaluate(sequences.at(i));
  }

  return scores;
}

template<class SEQUENCE_T, typename T>
DocumentScores BlockEvaluator<SEQUENCE_T, T>::evaluate(const DocumentSequences<SEQUENCE_T>& sequences) const {
  std::size_t n = sequences.size();

  if (n == 0 || n == 1)
    return DocumentScores{0};

  DocumentScores scores(n, 0);

  BlockSequences<SEQUENCE_T> left_block, right_block;
  Sequence<SEQUENCE_T> left_block_ts, right_block_ts;

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

    left_block = BlockSequences<SEQUENCE_T>(sequences.cbegin() + left_start_offset,
                                            sequences.cbegin() + left_end_offset);

    right_block = BlockSequences<SEQUENCE_T>(sequences.cbegin() + right_start_offset,
                                             sequences.cbegin() + right_end_offset);

    scores.at(gap) = compute_score(left_block, right_block);
  }

  return scores;
}

template<class SEQUENCE_T, typename T>
SEQUENCE_T BlockEvaluator<SEQUENCE_T, T>::create_sequence(const BlockSequences<SEQUENCE_T>& block) const {
  SEQUENCE_T result;

  for (auto const& ts : block) {
    result += ts;
  }

  return result;
}

template<class SEQUENCE_T, typename T>
Score BlockEvaluator<SEQUENCE_T, T>::compute_score(const BlockSequences<SEQUENCE_T>& left,
                                                   const BlockSequences<SEQUENCE_T>& right) const {
  SEQUENCE_T left_ts = create_sequence(left);
  SEQUENCE_T right_ts = create_sequence(right);

  std::size_t numerator = compute_numerator(left_ts, right_ts);
  double denominator = compute_denominator(left_ts, right_ts);

  return numerator / denominator;
}

template<class SEQUENCE_T, typename T>
std::size_t BlockEvaluator<SEQUENCE_T, T>::compute_numerator(const SEQUENCE_T& left,
                                                             const SEQUENCE_T& right) const {
  SEQUENCE_T ts = left * right;
  return sum(ts.get_values());
}

template<class SEQUENCE_T, typename T>
double BlockEvaluator<SEQUENCE_T, T>::compute_denominator(const SEQUENCE_T& left,
                                                          const SEQUENCE_T& right) const {
  std::vector<std::size_t> left_counts = left.get_values();
  std::vector<std::size_t> right_counts = right.get_values();

  std::size_t left_sq_sum = square_sum(left_counts);
  std::size_t right_sq_sum = square_sum(right_counts);

  return sqrt(left_sq_sum * right_sq_sum);
}

#endif // BLOCK_EVALUATOR_HPP
