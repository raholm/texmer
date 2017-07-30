#ifndef LEXICAL_EVALUATOR_H
#define LEXICAL_EVALUATOR_H

#include "def.h"
#include "token_sequence.h"
#include "vocabulary.h"

class LexicalEvaluator {
public:
  LexicalEvaluator() = default;

  virtual ~LexicalEvaluator() = default;

  virtual CorpusScores evaluate(const CorpusTokenSequences& token_sequences) const = 0;
  virtual DocumentScores evaluate(const DocumentTokenSequences& token_sequences) const = 0;

};

class BlockEvaluator : public LexicalEvaluator {
public:
  BlockEvaluator(std::size_t block_size);

  ~BlockEvaluator() = default;

  CorpusScores evaluate(const CorpusTokenSequences& token_sequences) const override;
  DocumentScores evaluate(const DocumentTokenSequences& token_sequences) const override;

private:
  std::size_t block_size_;

  TokenSequence create_token_sequence(const BlockTokenSequences& block) const;
  Score compute_score(const BlockTokenSequences& left, const BlockTokenSequences& right) const;
  std::size_t compute_numerator(const TokenSequence& left, const TokenSequence& right) const;
  double compute_denominator(const TokenSequence& left, const TokenSequence& right) const;

};

class VocabularyEvaluator : public LexicalEvaluator {
public:
  VocabularyEvaluator(std::size_t sentence_size);

  ~VocabularyEvaluator() = default;

  CorpusScores evaluate(const CorpusTokenSequences& token_sequences) const override;
  DocumentScores evaluate(const DocumentTokenSequences& token_sequences) const override;

private:
  std::size_t sentence_size_;

  std::vector<Vocabulary> get_vocabularies(const DocumentTokenSequences& token_sequences) const;

};

#endif // LEXICAL_EVALUATOR_H
