#include "token_sequence.h"

#include <iostream>
#include <algorithm>

TokenSequence::TokenSequence(const std::vector<Token>& tokens) {
  for (auto const &token : tokens) {
    insert_or_add_element(std::make_pair(token, 1));
  }
}

TokenSequence::TokenSequence(const Rcpp::StringVector& tokens) {
  for (auto const &token : tokens) {
    insert_or_add_element(std::make_pair(Rcpp::as<Token>(token), 1));
  }
}

TokenSequence TokenSequence::operator+(const TokenSequence& rhs) const {
  TokenSequence tmp(*this);
  return tmp += rhs;
}

TokenSequence& TokenSequence::operator+=(const TokenSequence& rhs) {
  for (auto const &p : rhs.type_count) {
    insert_or_add_element(p);
  }
  return *this;
}

std::size_t TokenSequence::length() const {
  return std::accumulate(std::begin(type_count), std::end(type_count), 0,
                         [](const std::size_t acc,
                            const std::pair<Token, std::size_t>& p) {
                           return acc + p.second;
                         });
}

Vocabulary TokenSequence::get_vocabulary() const {
  std::vector<Token> vocabulary;
  std::transform(type_count.cbegin(), type_count.cend(), std::back_inserter(vocabulary),
                 [](auto const& pair) {
                   return pair.first;
                 });
  return Vocabulary(vocabulary);
}

std::vector<std::size_t> TokenSequence::get_counts() const {
  std::vector<std::size_t> counts;
  std::transform(type_count.cbegin(), type_count.cend(), std::back_inserter(counts),
                 [](auto const& pair) {
                   return pair.second;
                 });
  return counts;
}

void TokenSequence::print() const {
  for (auto const &p : type_count) {
    std::cout << p.first << ": " << p.second << std::endl;
  }
  std::cout << std::endl;
}

void TokenSequence::insert_or_add_element(const std::pair<Token, std::size_t>& element) {
  if (!type_count.insert(element).second) {
    type_count[element.first] += element.second;
  }
}

// [[Rcpp::export]]
void tokensequence_test() {
  Rcpp::StringVector strings = Rcpp::StringVector::create("hello", "world", "hello", "what", "are", "you", "doing", "doing");

  TokenSequence ts1(strings);
  TokenSequence ts2(strings);
  TokenSequence ts3;

  std::cout << "ts1 (" << ts1.length() << "):" << std::endl;
  ts1.print();

  ts1 += ts2;

  std::cout << "ts1 += ts2 (" << ts1.length() << "):" << std::endl;
  ts1.print();

  ts3 = ts1 + ts2;

  std::cout << "ts1 + ts2 (" << ts3.length() << "):" << std::endl;
  ts3.print();

  std::cout << "ts1 vocabulary:" << std::endl;
  ts1.get_vocabulary().print();
}
