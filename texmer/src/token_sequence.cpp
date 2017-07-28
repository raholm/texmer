#include "token_sequence.h"

TokenSequence::TokenSequence(const Rcpp::StringVector& tokens) {
  for (unsigned i = 0; i < tokens.size(); ++i) {
    insert_or_add_element(std::make_pair(Rcpp::as<std::string>(tokens[i]), 1));
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
                            const std::pair<std::string, std::size_t>& p) {
                           return acc + p.second;
                         });
}

void TokenSequence::print() const {
  for (auto const &p : type_count) {
    std::cout << p.first << ": " << p.second << std::endl;
  }
  std::cout << std::endl;
}

void TokenSequence::insert_or_add_element(const std::pair<std::string, std::size_t>& element) {
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
}
