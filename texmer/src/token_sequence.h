#ifndef TEXMER_TOKEN_SEQUENCE_H_
#define TEXMER_TOKEN_SEQUENCE_H_

#include "sequence.h"
#include "vocabulary.h"
#include "def.h"

namespace texmer {

  class TokenSequence : public Sequence<String> {
  public:
    using BaseClass = Sequence<String>;
    using KeyType = BaseClass::KeyType;
    using ValueType = BaseClass::ValueType;

    TokenSequence() : BaseClass() {}
    TokenSequence(const TokenSequence& other) : BaseClass(other) {}
    TokenSequence(TokenSequence&& other) : BaseClass(std::move(other)) {}
    TokenSequence(const Vector<KeyType>& tokens) : BaseClass(tokens) {}

    ~TokenSequence() = default;

    inline TokenSequence& operator=(const TokenSequence& rhs) {
      BaseClass::operator=(rhs);
      return *this;
    }

    inline TokenSequence& operator=(TokenSequence&& rhs) {
      BaseClass::operator=(std::move(rhs));
      return *this;
    }

    inline TokenSequence operator+(const TokenSequence& rhs) const {
      TokenSequence copy(*this);
      return copy += rhs;
    }

    inline TokenSequence& operator+=(const TokenSequence& rhs) {
      BaseClass::operator+=(rhs);
      return *this;
    }

    inline TokenSequence operator*(const TokenSequence& rhs) const {
      TokenSequence copy(*this);
      return copy *= rhs;
    }

    inline TokenSequence& operator*=(const TokenSequence& rhs) {
      BaseClass::operator*=(rhs);
      return *this;
    }

    inline bool operator==(const TokenSequence& rhs) const {
      return BaseClass::operator==(rhs);
    }

    inline bool operator!=(const TokenSequence& rhs) const {
      return BaseClass::operator!=(rhs);
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
