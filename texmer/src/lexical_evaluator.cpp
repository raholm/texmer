#include "lexical_evaluator.h"

namespace texmer {

  CorpusScores VocabularyEvaluator::evaluate(const CorpusSequences<TokenSequence>& sequences) const {
    size_t n = sequences.size();
    CorpusScores scores(n);

    for (unsigned i = 0; i < n; ++i) {
      scores.at(i) = evaluate(sequences.at(i));
    }

    return scores;
  }

  DocumentScores VocabularyEvaluator::evaluate(const DocumentSequences<TokenSequence>& sequences) const {
    size_t n = sequences.size();

    if (n == 0 || n == 1)
      return DocumentScores{0};

    DocumentScores scores(n, 0);
    double denom = 2 * sentence_size_;
    size_t n_gaps = n - 1;

    Vector<Vocabulary> vocabularies = get_vocabularies(sequences);

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

  Vector<Vocabulary> VocabularyEvaluator::get_vocabularies(const DocumentSequences<TokenSequence>& sequences) const {
    size_t n = sequences.size();
    Vector<Vocabulary> vocabularies(n);

    for (unsigned i = 0; i < n; ++i) {
      vocabularies.at(i) = sequences.at(i).get_vocabulary();
    }

    return vocabularies;
  }

} // namespace texmer
