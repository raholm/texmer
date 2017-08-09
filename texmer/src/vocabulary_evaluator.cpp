#include "vocabulary_evaluator.h"

#include <exception>

#include "util.h"

VocabularyEvaluator::VocabularyEvaluator(std::size_t sentence_size)
  : sentence_size_(sentence_size)
{
  if (sentence_size_ < 1) {
    throw std::invalid_argument("Invalid sentence size: '" + std::to_string(sentence_size_) + "'.");
  }

}

CorpusScores VocabularyEvaluator::evaluate(const CorpusSequences<TokenSequence>& token_sequences) const {
  std::size_t n = token_sequences.size();
  CorpusScores scores(n);

  for (unsigned i = 0; i < n; ++i) {
    scores.at(i) = evaluate(token_sequences.at(i));
  }

  return scores;
}

DocumentScores VocabularyEvaluator::evaluate(const DocumentSequences<TokenSequence>& token_sequences) const {
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

std::vector<Vocabulary> VocabularyEvaluator::get_vocabularies(const DocumentSequences<TokenSequence>& token_sequences) const {
  std::size_t n = token_sequences.size();
  std::vector<Vocabulary> vocabularies(n);

  for (unsigned i = 0; i < n; ++i) {
    vocabularies.at(i) = token_sequences.at(i).get_vocabulary();
  }

  return vocabularies;
}
