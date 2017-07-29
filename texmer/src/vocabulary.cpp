#include "vocabulary.h"

Vocabulary::Vocabulary(const std::vector<Token>& tokens) {
  vocabulary_.insert(tokens.cbegin(), tokens.cend());
}

Vocabulary Vocabulary::operator+(const Vocabulary& rhs) const {
  Vocabulary tmp(*this);
  return tmp += rhs;
}

Vocabulary& Vocabulary::operator+=(const Vocabulary& rhs) {
  vocabulary_.insert(rhs.vocabulary_.cbegin(), rhs.vocabulary_.cend());
  return *this;
}

Vocabulary Vocabulary::operator-(const Vocabulary& rhs) const {
  Vocabulary tmp(*this);
  return tmp -= rhs;
}

Vocabulary& Vocabulary::operator-=(const Vocabulary& rhs) {
  vocabulary_.erase(rhs.vocabulary_.cbegin(), rhs.vocabulary_.cend());
  return *this;
}

bool Vocabulary::find(const Token& token) const {
  return vocabulary_.find(token) != vocabulary_.end();
}

std::size_t Vocabulary::length() const {
  return vocabulary_.size();
}

std::ostream& Vocabulary::print(std::ostream& out) const {
  out << "{";
  unsigned count = 0;
  for (auto const& token : vocabulary_) {
    out << token;
    if (++count != length())
      out << ", ";
  }
  out << "}";
  return out;
}
