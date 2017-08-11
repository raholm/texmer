#ifndef TEXMER_VOCABULARY_H_
#define TEXMER_VOCABULARY_H_

#include <iostream>
#include <algorithm>

#include "def.h"

namespace texmer {

  class Vocabulary {
  public:
    Vocabulary() = default;
    Vocabulary(const Vocabulary& v) = default;
    Vocabulary(Vocabulary&& v) = default;
    explicit Vocabulary(const Vector<Token>& tokens) {
      vocabulary_.insert(tokens.cbegin(), tokens.cend());
    }

    ~Vocabulary() = default;

    Vocabulary& operator=(const Vocabulary& other) = default;
    Vocabulary& operator=(Vocabulary&& other) = default;

    inline Vocabulary operator+(const Vocabulary& rhs) const {
      Vocabulary tmp(*this);
      return tmp += rhs;
    }

    inline Vocabulary& operator+=(const Vocabulary& rhs) {
      vocabulary_.insert(rhs.vocabulary_.cbegin(), rhs.vocabulary_.cend());
      return *this;
    }

    inline Vocabulary operator-(const Vocabulary& rhs) const {
      Vocabulary tmp(*this);
      return tmp -= rhs;
    }

    inline Vocabulary& operator-=(const Vocabulary& rhs) {
      Set<Token> tmp;
      std::set_difference(std::make_move_iterator(vocabulary_.begin()),
                          std::make_move_iterator(vocabulary_.end()),
                          rhs.vocabulary_.cbegin(),
                          rhs.vocabulary_.cend(),
                          std::inserter(tmp, tmp.begin()));
      vocabulary_.swap(tmp);
      return *this;
    }

    inline bool contains(const Token& token) const {
      return vocabulary_.find(token) != vocabulary_.end();
    }

    inline size_t size() const {
      return vocabulary_.size();
    }

    inline size_t length() const {
      return size();
    }

    std::ostream& print(std::ostream& out=std::cout) const;

  private:
    Set<Token> vocabulary_;

  };

} // namespace texmer

#endif // TEXMER_VOCABULARY_H_
