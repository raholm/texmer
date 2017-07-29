#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <Rcpp.h>

#include <vector>
#include <string>
#include <set>

class Vocabulary {
public:
  using Token = std::string;

  Vocabulary() = default;
  Vocabulary(const Vocabulary& v) = default;
  Vocabulary(Vocabulary&& v) = default;
  Vocabulary(const std::vector<Token>& tokens);
  Vocabulary(const Rcpp::StringVector& tokens);

  Vocabulary operator+(const Vocabulary& rhs) const;
  Vocabulary& operator+=(const Vocabulary& rhs);

  Vocabulary operator-(const Vocabulary& rhs) const;
  Vocabulary& operator-=(const Vocabulary& rhs);

  std::size_t length() const;

  void print() const;

private:
  std::set<Token> vocabulary;
};

#endif // VOCABULARY_H
