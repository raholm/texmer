#ifndef TOKEN_SEQUENCE_H
#define TOKEN_SEQUENCE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "sequence.h"
#include "vocabulary.h"

class TokenSequence : public Sequence<std::string> {
public:
  using base = Sequence<std::string>;
  using token = typename base::key;
  using count = typename base::value;

  TokenSequence() : base() {}
  TokenSequence(const TokenSequence& other) : base(other) {}
  TokenSequence(TokenSequence&& other) : base(std::move(other)) {}
  TokenSequence(const std::vector<token>& tokens) : base(tokens) {}

  ~TokenSequence() = default;

  inline TokenSequence& operator=(const TokenSequence& rhs) {
    base::operator=(rhs);
    return *this;
  }

  inline TokenSequence& operator=(TokenSequence&& rhs) {
    base::operator=(std::move(rhs));
    return *this;
  }

  inline TokenSequence operator+(const TokenSequence& rhs) const {
    TokenSequence copy(*this);
    return copy += rhs;
  }

  inline TokenSequence& operator+=(const TokenSequence& rhs) {
    base::operator+=(rhs);
    return *this;
  }

  inline TokenSequence operator*(const TokenSequence& rhs) const {
    TokenSequence copy(*this);
    return copy *= rhs;
  }

  inline TokenSequence& operator*=(const TokenSequence& rhs) {
    base::operator*=(rhs);
    return *this;
  }

  inline bool operator==(const TokenSequence& rhs) const {
    return base::operator==(rhs);
  }

  inline bool operator!=(const TokenSequence& rhs) const {
    return base::operator!=(rhs);
  }

  TokenSequence operator-(const Vocabulary& rhs) const;
  TokenSequence& operator-=(const Vocabulary& rhs);

  Vocabulary get_vocabulary() const;

};

using DocumentTokenSequences = DocumentSequences<TokenSequence>;
using CorpusTokenSequences = std::vector<DocumentTokenSequences>;
using BlockTokenSequences = DocumentTokenSequences;

#endif // TOKEN_SEQUENCE_H
