#ifndef TEXMER_LEXICAL_EVALUATOR_H_
#define TEXMER_LEXICAL_EVALUATOR_H_

#include <exception>
#include <type_traits>
#include <string>

#include "def.h"
#include "sequence.h"
#include "token_sequence.h"
#include "topic_indicator_sequence.h"
#include "vocabulary.h"

namespace texmer {

  class LexicalEvaluatorBase {
  public:
    virtual ~LexicalEvaluatorBase() = default;

  };

  template<class Sequence>
  class LexicalEvaluator : public LexicalEvaluatorBase {
    static_assert(std::is_base_of<SequenceBase, Sequence>::value,
                  "Sequence must be a descendant of SequenceBase");

  public:
    LexicalEvaluator() = default;

    virtual ~LexicalEvaluator() = default;

    virtual CorpusScores evaluate(const CorpusSequences<Sequence>& sequences) const = 0;
    virtual DocumentScores evaluate(const DocumentSequences<Sequence>& sequences) const = 0;

  };

  template<class Sequence>
  class BlockEvaluator : public LexicalEvaluator<Sequence> {
  public:
    explicit BlockEvaluator(size_t block_size)
      : block_size_{block_size} {
      if (block_size_ < 1) {
        throw std::invalid_argument("Invalid block size: '" + std::to_string(block_size_) + "'.");
      }
    }


    ~BlockEvaluator() = default;

    CorpusScores evaluate(const CorpusSequences<Sequence>& sequences) const override;
    DocumentScores evaluate(const DocumentSequences<Sequence>& sequences) const override;

  private:
    size_t block_size_;

    Sequence create_sequence(const BlockSequences<Sequence>& block) const;
    Score compute_score(const BlockSequences<Sequence>& left,
                        const BlockSequences<Sequence>& right) const;
    size_t compute_numerator(const Sequence& left, const Sequence& right) const;
    double compute_denominator(const Sequence& left, const Sequence& right) const;

  };

  class VocabularyEvaluator : public LexicalEvaluator<TokenSequence> {
  public:
    explicit VocabularyEvaluator(size_t sentence_size)
      : sentence_size_{sentence_size} {
      if (sentence_size_ < 1) {
        throw std::invalid_argument("Invalid sentence size: '" + std::to_string(sentence_size_) + "'.");
      }
    }

    ~VocabularyEvaluator() = default;

    CorpusScores evaluate(const CorpusSequences<TokenSequence>& sequences) const override;
    DocumentScores evaluate(const DocumentSequences<TokenSequence>& sequences) const override;

  private:
    size_t sentence_size_;

    Vector<Vocabulary> get_vocabularies(const DocumentSequences<TokenSequence>& sequences) const;

  };

  using TextTileBlockEvaluator = BlockEvaluator<TokenSequence>;
  using TopicTileBlockEvaluator = BlockEvaluator<TopicIndicatorSequence>;

  using TextTileVocabularyEvaluator = VocabularyEvaluator;

} // namespace texmer

#include "lexical_evaluator.hpp"

#endif // TEXMER_LEXICAL_EVALUATOR_H_
