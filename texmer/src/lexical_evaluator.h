#ifndef LEXICAL_EVALUATOR_H
#define LEXICAL_EVALUATOR_H

#include <type_traits>

#include "def.h"
#include "sequence.h"
#include "vocabulary.h"

class LexicalEvaluator {
public:
  virtual ~LexicalEvaluator() = default;

};

template<class SEQUENCE_T, typename T>
class GenericLexicalEvaluator : public LexicalEvaluator {
  static_assert(std::is_base_of<Sequence<T>, SEQUENCE_T>::value,
                "Sequence must be a descendant of Sequence<T>");

public:
  GenericLexicalEvaluator() = default;

  virtual ~GenericLexicalEvaluator() = default;

  virtual CorpusScores evaluate(const CorpusSequences<SEQUENCE_T>& token_sequences) const = 0;
  virtual DocumentScores evaluate(const DocumentSequences<SEQUENCE_T>& token_sequences) const = 0;

};

#endif // LEXICAL_EVALUATOR_H
