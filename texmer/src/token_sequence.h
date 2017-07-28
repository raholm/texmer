#ifndef TOKEN_SEQUENCE_H
#define TOKEN_SEQUENCE_H

#include <Rcpp.h>

#include <iostream>
#include <map>
#include <string>

class TokenSequence {
public:
  TokenSequence() = default;
  TokenSequence(const TokenSequence& ts) = default;
  TokenSequence(TokenSequence&& ts) = default;
  TokenSequence(const Rcpp::StringVector& tokens);

  ~TokenSequence() = default;

  TokenSequence& operator=(const TokenSequence& other) = default;
  TokenSequence& operator=(TokenSequence&& other) = default;

  TokenSequence operator+(const TokenSequence& rhs) const;
  TokenSequence& operator+=(const TokenSequence& rhs);

  std::size_t length() const;

  void print() const;

private:
  std::map<std::string, std::size_t> type_count;

  void insert_or_add_element(const std::pair<std::string, std::size_t>& element);

};

#endif // TOKEN_SEQUENCE_H
