#ifndef VOCABULARY_EVALUATOR_H
#define VOCABULARY_EVALUATOR_H

#include "lexical_evaluator.h"
#include "token_sequence.h"

class VocabularyEvaluator : public GenericLexicalEvaluator<TokenSequence, std::string> {
 public:
  VocabularyEvaluator(std::size_t sentence_size);

  ~VocabularyEvaluator() = default;

  CorpusScores evaluate(const CorpusSequences<TokenSequence>& token_sequences) const override;
  DocumentScores evaluate(const DocumentSequences<TokenSequence>& token_sequences) const override;

 private:
  std::size_t sentence_size_;

  std::vector<Vocabulary> get_vocabularies(const DocumentSequences<TokenSequence>& token_sequences) const;

};

#endif // VOCABULARY_EVALUATOR_H
