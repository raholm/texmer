#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include <catch.hpp>

#include "def.h"
#include "vocabulary.h"

template<typename T>
void check_equality(const std::vector<T>& v1, const std::vector<T>& v2) {
  REQUIRE(v1.size() == v2.size());

  for (unsigned i = 0; i < v1.size(); ++i)
    REQUIRE(v1.at(i) == v2.at(i));
}

inline void check_contains(const Vocabulary& vocabulary, const std::vector<std::string>& tokens) {
  for (auto const& token : tokens)
    REQUIRE(vocabulary.contains(token));
}

inline void check_not_contains(const Vocabulary& vocabulary, const Document& document) {
  for (auto const& token : document)
    REQUIRE(!vocabulary.contains(token));
}

inline void check_equality(const DoubleVector& s1, const DoubleVector& s2) {
  REQUIRE(s1.size() == s2.size());

  for (unsigned i = 0; i < s1.size(); ++i)
    REQUIRE(s1.at(i) == Approx(s2.at(i)));
}

inline void check_equality(const DoubleMatrix& s1, const DoubleMatrix& s2) {
  REQUIRE(s1.size() == s2.size());

  for (unsigned i = 0; i < s1.size(); ++i)
    check_equality(s1.at(i), s2.at(i));
}

inline void check_size(const Vocabulary& vocabulary, std::size_t size) {
  REQUIRE(vocabulary.size() == size);
  REQUIRE(vocabulary.size() == vocabulary.length());
}


#endif
