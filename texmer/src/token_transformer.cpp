#include "token_transformer.h"

#include <exception>
#include <cmath>

TokenTransformer::TokenTransformer(std::size_t sentence_size)
  : sentence_size_(sentence_size)
{
  if (sentence_size_ < 1) {
    throw std::invalid_argument("Invalid sentence size: '" + std::to_string(sentence_size_) + "'.");
  }
}

CorpusTokenSequences TokenTransformer::transform(const Corpus& tokens, const Document& stopwords) const {
    return transform(tokens, Vocabulary(stopwords));
}

CorpusTokenSequences TokenTransformer::transform(const Corpus& tokens, const Vocabulary& stopwords) const {
  CorpusTokenSequences token_sequences(tokens.size());

  for (unsigned i = 0; i < tokens.size(); ++i) {
    token_sequences.at(i) = transform(tokens.at(i), stopwords);
  }

  return token_sequences;
}

DocumentTokenSequences TokenTransformer::transform(const Document& tokens, const Document& stopwords) const {
    return transform(tokens, Vocabulary(stopwords));
}

DocumentTokenSequences TokenTransformer::transform(const Document& tokens, const Vocabulary& stopwords) const {
  std::size_t n_segs = ceil(tokens.size() / sentence_size_);
  DocumentTokenSequences token_sequences;

  std::size_t start, end;
  Document segment;
  TokenSequence token_sequence;

  for (unsigned i = 0; i < n_segs; ++i) {
    start = i * sentence_size_;
    end = std::min(start + sentence_size_, tokens.size());

    segment = Document(tokens.begin() + start, tokens.begin() + end);

    token_sequence = TokenSequence(segment);
    token_sequence -= stopwords;

    if (token_sequence.length() == 0)
      continue;

    token_sequences.push_back(token_sequence);
  }

  return token_sequences;
}
