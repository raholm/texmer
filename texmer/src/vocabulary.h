#ifndef VOCABULARY_H
#define VOCABULARY_H

#include <iostream>
#include <string>
#include <vector>
#include <set>

class Vocabulary {
public:
  using Token = std::string;

  Vocabulary() = default;
  Vocabulary(const Vocabulary& v) = default;
  Vocabulary(Vocabulary&& v) = default;
  Vocabulary(const std::vector<Token>& tokens);

  ~Vocabulary() = default;

  Vocabulary& operator=(const Vocabulary& other) = default;
  Vocabulary& operator=(Vocabulary&& other) = default;

  Vocabulary operator+(const Vocabulary& rhs) const;
  Vocabulary& operator+=(const Vocabulary& rhs);

  Vocabulary operator-(const Vocabulary& rhs) const;
  Vocabulary& operator-=(const Vocabulary& rhs);

  bool is_in(const Token& token) const;

  std::size_t size() const;
  std::size_t length() const;

  std::ostream& print(std::ostream& out=std::cout) const;

private:
  std::set<Token> vocabulary_;

};

#endif // VOCABULARY_H
