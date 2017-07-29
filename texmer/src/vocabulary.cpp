#include "vocabulary.h"

#include <iostream>

Vocabulary::Vocabulary(const std::vector<Token>& tokens) {
  vocabulary.insert(tokens.cbegin(), tokens.cend());
}

Vocabulary Vocabulary::operator+(const Vocabulary& rhs) const {
  Vocabulary tmp(*this);
  return tmp += rhs;
}

Vocabulary& Vocabulary::operator+=(const Vocabulary& rhs) {
  vocabulary.insert(rhs.vocabulary.cbegin(), rhs.vocabulary.cend());
  return *this;
}

Vocabulary Vocabulary::operator-(const Vocabulary& rhs) const {
  Vocabulary tmp(*this);
  return tmp -= rhs;
}

Vocabulary& Vocabulary::operator-=(const Vocabulary& rhs) {
  vocabulary.erase(rhs.vocabulary.cbegin(), rhs.vocabulary.cend());
  return *this;
}

bool Vocabulary::find(const Token& token) const {
  return vocabulary.find(token) != vocabulary.end();
}

std::size_t Vocabulary::length() const {
  return vocabulary.size();
}

void Vocabulary::print() const {
  for (auto const& token : vocabulary) {
    std::cout << token << " ";
  }
  std::cout << std::endl;
}
