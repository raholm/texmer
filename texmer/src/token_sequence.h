#ifndef TOKEN_SEQUENCE_H
#define TOKEN_SEQUENCE_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "vocabulary.h"

class TokenSequence {
public:
  using Token = std::string;

  TokenSequence() = default;
  TokenSequence(const TokenSequence& ts) = default;
  TokenSequence(TokenSequence&& ts) = default;
  TokenSequence(const std::vector<Token>& tokens);

  ~TokenSequence() = default;

  TokenSequence& operator=(const TokenSequence& other) = default;
  TokenSequence& operator=(TokenSequence&& other) = default;

  TokenSequence operator+(const TokenSequence& rhs) const;
  TokenSequence& operator+=(const TokenSequence& rhs);

  TokenSequence operator*(const TokenSequence& rhs) const;
  TokenSequence& operator*=(const TokenSequence& rhs);

  TokenSequence operator-(const Vocabulary& rhs) const;
  TokenSequence& operator-=(const Vocabulary& rhs);

  std::size_t size() const;
  std::size_t length() const;

  Vocabulary get_vocabulary() const;
  std::vector<std::size_t> get_counts() const;

  std::ostream& print(std::ostream& out=std::cout) const;

private:
  std::map<Token, std::size_t> type_count_;

  void insert_or_add_element(const std::pair<Token, std::size_t>& element);

};

#endif // TOKEN_SEQUENCE_H
