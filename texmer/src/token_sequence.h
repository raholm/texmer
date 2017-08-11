#ifndef TEXMER_TOKEN_SEQUENCE_H_
#define TEXMER_TOKEN_SEQUENCE_H_

#include "sequence.h"
#include "vocabulary.h"
#include "def.h"

namespace texmer {

  class TokenSequence : public Sequence<String> {
  public:
    using base_class = Sequence<String>;
    using key = base_class::key;
    using value = base_class::value;

    TokenSequence() : base_class() {}
    TokenSequence(const TokenSequence& other) : base_class(other) {}
    TokenSequence(TokenSequence&& other) : base_class(std::move(other)) {}
    TokenSequence(const Vector<key>& tokens) : base_class(tokens) {}

    ~TokenSequence() = default;

    inline TokenSequence& operator=(const TokenSequence& rhs) {
      base_class::operator=(rhs);
      return *this;
    }

    inline TokenSequence& operator=(TokenSequence&& rhs) {
      base_class::operator=(std::move(rhs));
      return *this;
    }

    inline TokenSequence operator+(const TokenSequence& rhs) const {
      TokenSequence copy(*this);
      return copy += rhs;
    }

    inline TokenSequence& operator+=(const TokenSequence& rhs) {
      base_class::operator+=(rhs);
      return *this;
    }

    inline TokenSequence operator*(const TokenSequence& rhs) const {
      TokenSequence copy(*this);
      return copy *= rhs;
    }

    inline TokenSequence& operator*=(const TokenSequence& rhs) {
      base_class::operator*=(rhs);
      return *this;
    }

    inline bool operator==(const TokenSequence& rhs) const {
      return base_class::operator==(rhs);
    }

    inline bool operator!=(const TokenSequence& rhs) const {
      return base_class::operator!=(rhs);
    }

    inline TokenSequence operator-(const Vocabulary& rhs) const {
      TokenSequence tmp(*this);
      return tmp -= rhs;
    }

    inline TokenSequence& operator-=(const Vocabulary& rhs) {
      for (auto it = map_.cbegin(); it != map_.cend();) {
        if (rhs.contains(it->first)) it = map_.erase(it);
        else ++it;
      }
      return *this;
    }

    inline Vocabulary get_vocabulary() const {
      return Vocabulary(get_keys());
    }

  };

  using DocumentTokenSequences = DocumentSequences<TokenSequence>;
  using CorpusTokenSequences = Vector<DocumentTokenSequences>;
  using BlockTokenSequences = DocumentTokenSequences;

} // namespace texmer

#endif // TEXMER_TOKEN_SEQUENCE_H_
