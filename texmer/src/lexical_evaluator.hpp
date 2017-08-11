#ifndef TEXMER_LEXICAL_EVALUATOR_HPP_
#define TEXMER_LEXICAL_EVALUATOR_HPP_

#include "lexical_evaluator.h"

#include "util.h"

namespace texmer {

  template<class Sequence>
  CorpusScores BlockEvaluator<Sequence>::evaluate(const CorpusSequences<Sequence>& sequences) const {
    size_t n = sequences.size();
    CorpusScores scores(n);

    for (unsigned i = 0; i < n; ++i)
      scores.at(i) = evaluate(sequences.at(i));

    return scores;
  }

  template<class Sequence>
  DocumentScores BlockEvaluator<Sequence>::evaluate(const DocumentSequences<Sequence>& sequences) const {
    size_t n = sequences.size();

    if (n == 0 || n == 1)
      return DocumentScores{0};

    DocumentScores scores(n, 0);
    BlockSequences<Sequence> left_block, right_block;

    size_t current_block_size;
    size_t left_start_offset, left_end_offset;
    size_t right_start_offset, right_end_offset;
    size_t n_gaps = n - 1;
    size_t next_gap;

    for (size_t gap = 0; gap < n_gaps; ++gap) {
      next_gap = gap + 1;

      current_block_size = std::min({gap, block_size_, n_gaps - next_gap});

      left_start_offset = gap - current_block_size;
      left_end_offset = next_gap;

      right_start_offset = left_end_offset;
      right_end_offset = right_start_offset + current_block_size + 1;

      left_block = BlockSequences<Sequence>(sequences.cbegin() + left_start_offset,
                                            sequences.cbegin() + left_end_offset);

      right_block = BlockSequences<Sequence>(sequences.cbegin() + right_start_offset,
                                             sequences.cbegin() + right_end_offset);

      scores.at(gap) = compute_score(left_block, right_block);
    }

    return scores;
  }

  template<class Sequence>
  Sequence BlockEvaluator<Sequence>::create_sequence(const BlockSequences<Sequence>& block) const {
    Sequence result;

    for (auto const& ts : block)
      result += ts;

    return result;
  }

  template<class Sequence>
  Score BlockEvaluator<Sequence>::compute_score(const BlockSequences<Sequence>& left,
                                                const BlockSequences<Sequence>& right) const {
    Sequence left_ts = create_sequence(left);
    Sequence right_ts = create_sequence(right);

    size_t numerator = compute_numerator(left_ts, right_ts);
    double denominator = compute_denominator(left_ts, right_ts);

    return numerator / denominator;
  }

  template<class Sequence>
  size_t BlockEvaluator<Sequence>::compute_numerator(const Sequence& left,
                                                     const Sequence& right) const {
    Sequence ts = left * right;
    return sum(ts.get_values());
  }

  template<class Sequence>
  double BlockEvaluator<Sequence>::compute_denominator(const Sequence& left,
                                                       const Sequence& right) const {
    Vector<size_t> left_counts = left.get_values();
    Vector<size_t> right_counts = right.get_values();

    size_t left_sq_sum = square_sum(left_counts);
    size_t right_sq_sum = square_sum(right_counts);

    return sqrt(left_sq_sum * right_sq_sum);
  }

} // namespace texmer

#endif // TEXMER_LEXICAL_EVALUATOR_HPP_
