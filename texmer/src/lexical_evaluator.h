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
#include "util.h"

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
      : block_size_{block_size}
    {
      if (block_size_ < 1) {
        throw std::invalid_argument("Invalid block size: '" + std::to_string(block_size_) + "'.");
      }
    }


    ~BlockEvaluator() = default;

    CorpusScores evaluate(const CorpusSequences<Sequence>& sequences) const override {
      size_t n = sequences.size();
      CorpusScores scores(n);

      for (unsigned i = 0; i < n; ++i)
        scores.at(i) = evaluate(sequences.at(i));

      return scores;
    }

    DocumentScores evaluate(const DocumentSequences<Sequence>& sequences) const override {
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

  private:
    size_t block_size_;

    Sequence create_sequence(const BlockSequences<Sequence>& block) const {
      Sequence result;

      for (auto const& ts : block)
        result += ts;

      return result;
    }

    Score compute_score(const BlockSequences<Sequence>& left,
                        const BlockSequences<Sequence>& right) const {
      Sequence left_ts = create_sequence(left);
      Sequence right_ts = create_sequence(right);

      size_t numerator = compute_numerator(left_ts, right_ts);
      double denominator = compute_denominator(left_ts, right_ts);

      return numerator / denominator;
    }

    size_t compute_numerator(const Sequence& left, const Sequence& right) const {
      Sequence ts = left * right;
      return sum(ts.get_values());
    }

    double compute_denominator(const Sequence& left, const Sequence& right) const {
      Vector<size_t> left_counts = left.get_values();
      Vector<size_t> right_counts = right.get_values();

      size_t left_sq_sum = square_sum(left_counts);
      size_t right_sq_sum = square_sum(right_counts);

      return sqrt(left_sq_sum * right_sq_sum);
    }

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

#endif // TEXMER_LEXICAL_EVALUATOR_H_
