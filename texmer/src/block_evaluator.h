#ifndef BLOCK_EVALUATOR_H
#define BLOCK_EVALUATOR_H

#include "lexical_evaluator.h"

template<class SEQUENCE_T, typename T>
class BlockEvaluator : public GenericLexicalEvaluator<SEQUENCE_T, T> {
public:
  BlockEvaluator(std::size_t block_size);

  ~BlockEvaluator() = default;

  CorpusScores evaluate(const CorpusSequences<SEQUENCE_T>& sequences) const override;
  DocumentScores evaluate(const DocumentSequences<SEQUENCE_T>& sequences) const override;

private:
  std::size_t block_size_;

  SEQUENCE_T create_sequence(const BlockSequences<SEQUENCE_T>& block) const;
  Score compute_score(const BlockSequences<SEQUENCE_T>& left, const BlockSequences<SEQUENCE_T>& right) const;
  std::size_t compute_numerator(const SEQUENCE_T& left, const SEQUENCE_T& right) const;
  double compute_denominator(const SEQUENCE_T& left, const SEQUENCE_T& right) const;

};

#include "block_evaluator.hpp"

#endif // BLOCK_EVALUATOR_H
