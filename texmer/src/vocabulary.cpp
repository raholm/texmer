#include "vocabulary.h"

#include <iostream>
#include <algorithm>

Vocabulary::Vocabulary(const std::vector<Token>& tokens) {
  vocabulary.insert(tokens.cbegin(), tokens.cend());
}

Vocabulary::Vocabulary(const Rcpp::StringVector& tokens) {
  for (auto const &token : tokens) {
    vocabulary.insert(Rcpp::as<Token>(token));
  }
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

std::size_t Vocabulary::length() const {
  return vocabulary.size();
}

void Vocabulary::print() const {
  for (auto const& token : vocabulary) {
    std::cout << token << " ";
  }
  std::cout << std::endl;
}
