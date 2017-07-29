#include "vocabulary.h"

#include <algorithm>

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
  std::set<Token> tmp;
  std::set_difference(std::make_move_iterator(vocabulary_.begin()),
                      std::make_move_iterator(vocabulary_.end()),
                      rhs.vocabulary_.cbegin(),
                      rhs.vocabulary_.cend(),
                      std::inserter(tmp, tmp.begin()));
  vocabulary_.swap(tmp);
  return *this;
}

bool Vocabulary::find(const Token& token) const {
  return vocabulary_.find(token) != vocabulary_.end();
}

std::size_t Vocabulary::size() const {
  return vocabulary_.size();
}

std::size_t Vocabulary::length() const {
  return size();
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
