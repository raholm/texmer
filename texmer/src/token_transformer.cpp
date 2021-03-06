#include "token_transformer.h"

#include <cmath>

namespace texmer {

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
    size_t n_segs = ceil(tokens.size() / sentence_size_);
    DocumentTokenSequences token_sequences;
    token_sequences.reserve(n_segs);

    size_t start, end;
    Document segment;
    TokenSequence token_sequence;

    for (unsigned i = 0; i < n_segs; ++i) {
      start = i * sentence_size_;
      end = std::min(start + sentence_size_, tokens.size());

      segment = Document(tokens.begin() + start, tokens.begin() + end);

      token_sequence = TokenSequence(segment);
      token_sequence -= stopwords;

      // TODO: Is this what we want? Maybe there is sentence_size many stopwords
      // or at the end part.
      if (token_sequence.length() == 0)
        continue;

      token_sequences.push_back(token_sequence);
    }

    return token_sequences;
  }

} // namespace texmer
