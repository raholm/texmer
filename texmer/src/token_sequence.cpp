#include "token_sequence.h"

#include <iostream>
#include <algorithm>
#include <numeric>

TokenSequence::TokenSequence(const std::vector<Token>& tokens) {
  for (auto const &token : tokens) {
    insert_or_add_element(std::make_pair(token, 1));
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

TokenSequence TokenSequence::operator*(const TokenSequence& rhs) const {
  TokenSequence tmp(*this);
  return tmp *= rhs;
}

TokenSequence& TokenSequence::operator*=(const TokenSequence& rhs) {
  for (auto const& p : rhs.type_count) {
    auto it = type_count.find(p.first);
    if (it != type_count.end()) {
      type_count.at(it->first) = it->second * p.second;
    }
  }
  return *this;
}

TokenSequence TokenSequence::operator-(const Vocabulary& rhs) const {
  TokenSequence tmp(*this);
  return tmp -= rhs;
}

TokenSequence& TokenSequence::operator-=(const Vocabulary& rhs) {
  for (auto it = type_count.cbegin(); it != type_count.cend();) {
    if (rhs.find(it->first)) it = type_count.erase(it);
    else ++it;
  }
  return *this;
}


std::size_t TokenSequence::length() const {
  return std::accumulate(type_count.cbegin(), type_count.cend(), 0,
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
