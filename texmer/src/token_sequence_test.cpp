#include <testthat.h>

#include "token_sequence.h"
#include "test_helper.h"

namespace texmer {
  namespace test {

    context("token sequence") {
      test_that("token sequence given tokens construct correctly") {
        Vector<TokenSequence::key> tokens{"hello", "world", "hello"};

        TokenSequence ts{tokens};
        expect_true(ts.size() == 2);
        expect_true(ts.length() == 3);
      }

      test_that("token sequence given a copy construct correctly") {
        Vector<TokenSequence::key> tokens{"hello", "world", "hello"};
        TokenSequence copy(tokens);
        expect_true(copy.size() == 2);
        expect_true(copy.length() == 3);

        TokenSequence ts{copy};
        expect_true(ts.size() == copy.size());
        expect_true(ts.length() == copy.length());
      }

      test_that("token sequence given a moved copy construct correctly") {
        Vector<TokenSequence::key> tokens{"hello", "world", "hello"};
        TokenSequence copy(tokens);
        expect_true(copy.size() == 2);
        expect_true(copy.length() == 3);

        TokenSequence ts{std::move(copy)};
        expect_true(ts.size() == 2);
        expect_true(ts.length() == 3);
      }

      test_that("token sequence have getters") {
        TokenSequence ts({"hello", "world", "hello"});

        auto counts = ts.get_values();
        std::sort(counts.begin(), counts.end());
        check_equality(counts, {1, 2});

        auto vocabulary = ts.get_vocabulary();
        check_contains(vocabulary, {"hello", "world"});
      }

      test_that("token sequences can be compared") {
        TokenSequence ts1;
        TokenSequence ts2;
        expect_true(ts1 == ts2);

        TokenSequence ts3({"hello", "world", "hello"});
        TokenSequence ts4({"hello", "world", "hello"});
        expect_true(ts3 == ts4);

        TokenSequence ts5({"hello", "world", "hello"});
        TokenSequence ts6({"hello", "hello", "world"});
        expect_true(ts5 == ts6);

        TokenSequence ts7({"hello", "world", "hello"});
        TokenSequence ts8({"hello", "hello"});
        expect_true(ts7 != ts8);
      }

      test_that("token sequences can be added") {
        TokenSequence ts1({"hello", "world", "hello", "hello"});
        TokenSequence ts2({"hello", "hello", "what", "what"});
        TokenSequence ts3;

        ts3 = ts1 + ts2;

        auto counts = ts3.get_values();
        std::sort(counts.begin(), counts.end());
        check_equality(counts, {1, 2, 5});

        auto vocabulary = ts3.get_vocabulary();
        check_contains(vocabulary, {"hello", "world", "what"});

      }

      test_that("token sequences can be subtracted by vocabulary") {
        TokenSequence ts1({"hello", "world", "hello", "hello"});
        TokenSequence ts2({"hello", "hello", "what", "what"});
        TokenSequence ts3;
        Vocabulary v({"hello", "what"});

        ts3 = ts1 - v;

        auto counts = ts3.get_values();
        std::sort(counts.begin(), counts.end());
        check_equality(counts, {1});

        auto vocabulary = ts3.get_vocabulary();
        check_contains(vocabulary, {"world"});
        check_not_contains(vocabulary, {"hello", "what"});
      }

      test_that("token sequences can be multiplied") {
        TokenSequence ts1({"hello", "world", "hello", "hello"});
        TokenSequence ts2({"hello", "hello", "what", "what"});
        TokenSequence ts3;

        ts3 = ts1 * ts2;

        auto counts = ts3.get_values();
        std::sort(counts.begin(), counts.end());
        check_equality(counts, {6});

        auto vocabulary = ts3.get_vocabulary();
        check_contains(vocabulary, {"hello"});
        check_not_contains(vocabulary, {"world", "what"});      }
    }

  } // namespace test
} // namespace texmer
