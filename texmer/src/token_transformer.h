#ifndef TOKEN_TRANSFORMER_H
#define TOKEN_TRANSFORMER_H

#include "def.h"
#include "token_sequence.h"

class TokenTransformer {
 public:
  TokenTransformer(std::size_t sentence_size);

  ~TokenTransformer() = default;

  CorpusTokenSequences transform(const Corpus& tokens, const Document& stopwords) const;
  CorpusTokenSequences transform(const Corpus& tokens, const Vocabulary& stopwords) const;

  DocumentTokenSequences transform(const Document& tokens, const Document& stopwords) const;
  DocumentTokenSequences transform(const Document& tokens, const Vocabulary& stopwords) const;

private:
  std::size_t sentence_size_;

};

#endif // TOKEN_TRANSFORMER_H
