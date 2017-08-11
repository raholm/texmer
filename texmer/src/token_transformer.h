#ifndef TEXMER_TOKEN_TRANSFORMER_H_
#define TEXMER_TOKEN_TRANSFORMER_H_

#include <exception>
#include <string>

#include "def.h"
#include "token_sequence.h"

namespace texmer {

  class TokenTransformer {
  public:
    explicit TokenTransformer(size_t sentence_size)
      : sentence_size_(sentence_size) {
      if (sentence_size_ < 1) {
        throw std::invalid_argument("Invalid sentence size: '" + std::to_string(sentence_size_) + "'.");
      }
    }


    ~TokenTransformer() = default;

    CorpusTokenSequences transform(const Corpus& tokens, const Document& stopwords) const;
    CorpusTokenSequences transform(const Corpus& tokens, const Vocabulary& stopwords) const;

    DocumentTokenSequences transform(const Document& tokens, const Document& stopwords) const;
    DocumentTokenSequences transform(const Document& tokens, const Vocabulary& stopwords) const;

  private:
    size_t sentence_size_;

  };

} // namespace texmer

#endif // TEXMER_TOKEN_TRANSFORMER_H_
