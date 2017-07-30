#include "token_sequence.h"

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
  for (auto const &p : rhs.type_count_) {
    insert_or_add_element(p);
  }
  return *this;
}

TokenSequence TokenSequence::operator*(const TokenSequence& rhs) const {
  TokenSequence tmp(*this);
  return tmp *= rhs;
}

TokenSequence& TokenSequence::operator*=(const TokenSequence& rhs) {
  for (auto const& p : rhs.type_count_) {
    auto it = type_count_.find(p.first);
    if (it != type_count_.end()) {
      type_count_.at(it->first) = it->second * p.second;
    }
  }
  return *this;
}

TokenSequence TokenSequence::operator-(const Vocabulary& rhs) const {
  TokenSequence tmp(*this);
  return tmp -= rhs;
}

TokenSequence& TokenSequence::operator-=(const Vocabulary& rhs) {
  for (auto it = type_count_.cbegin(); it != type_count_.cend();) {
    if (rhs.is_in(it->first)) it = type_count_.erase(it);
    else ++it;
  }
  return *this;
}

std::size_t TokenSequence::size() const {
  return type_count_.size();
}

std::size_t TokenSequence::length() const {
  return std::accumulate(type_count_.cbegin(), type_count_.cend(), 0,
                         [](const std::size_t acc,
                            const std::pair<Token, std::size_t>& p) {
                           return acc + p.second;
                         });
}

Vocabulary TokenSequence::get_vocabulary() const {
  std::vector<Token> vocabulary;
  std::transform(type_count_.cbegin(), type_count_.cend(), std::back_inserter(vocabulary),
                 [](auto const& pair) {
                   return pair.first;
                 });
  return Vocabulary(vocabulary);
}

std::vector<std::size_t> TokenSequence::get_counts() const {
  std::vector<std::size_t> counts;
  std::transform(type_count_.cbegin(), type_count_.cend(), std::back_inserter(counts),
                 [](auto const& pair) {
                   return pair.second;
                 });
  return counts;
}

std::ostream& TokenSequence::print(std::ostream& out) const {
  out << "{";
  unsigned count = 0;
  for (auto const& p : type_count_) {
    out << p.first << ": " << p.second;
    if (++count != type_count_.size())
      out << ", ";
  }
  out << "}";
  return out;
}

void TokenSequence::insert_or_add_element(const std::pair<Token, std::size_t>& element) {
  if (!type_count_.insert(element).second) {
    type_count_[element.first] += element.second;
  }
}
