#include <testthat.h>

#include "topic_indicator_sequence.h"
#include "test_helper.h"

namespace texmer {
  namespace test {

    context("topic indicator sequence") {
      test_that("topic indicator sequence given topic indicators constructs correctly") {
        Vector<TopicIndicatorSequence::key> topic_indicators{1, 2, 1};

        TopicIndicatorSequence tis{topic_indicators};
        expect_true(tis.size() == 2);
        expect_true(tis.length() == 3);
      }

      test_that("topic indicator given a copy constructs correctly")  {
        Vector<TopicIndicatorSequence::key> topic_indicators{1, 2, 1};
        TopicIndicatorSequence copy(topic_indicators);
        expect_true(copy.size() == 2);
        expect_true(copy.length() == 3);

        TopicIndicatorSequence tis{copy};
        expect_true(tis.size() == copy.size());
        expect_true(tis.length() == copy.length());
      }

      test_that("topic indicator given a moved copy constructs correctly")  {
        Vector<TopicIndicatorSequence::key> topic_indicators{1, 2, 1};
        TopicIndicatorSequence copy(topic_indicators);
        expect_true(copy.size() == 2);
        expect_true(copy.length() == 3);

        TopicIndicatorSequence tis{std::move(copy)};
        expect_true(tis.size() == 2);
        expect_true(tis.length() == 3);
      }

      test_that("topic indicator sequence has getters") {
        TopicIndicatorSequence tis({1, 2, 1});

        auto counts = tis.get_values();
        std::sort(counts.begin(), counts.end());
        check_equality(counts, {1, 2});

        auto keys = tis.get_keys();
        std::sort(keys.begin(), keys.end());
        check_equality(keys, {1, 2});
      }

      test_that("topic indicators sequences can be compared") {
        TopicIndicatorSequence tis1;
        TopicIndicatorSequence tis2;
        expect_true(tis1 == tis2);

        TopicIndicatorSequence tis3({1, 2, 1});
        TopicIndicatorSequence tis4({1, 2, 1});
        expect_true(tis3 == tis4);

        TopicIndicatorSequence tis5({1, 2, 1});
        TopicIndicatorSequence tis6({1, 1, 2});
        expect_true(tis5 == tis6);

        TopicIndicatorSequence tis7({1, 2, 1});
        TopicIndicatorSequence tis8({1, 1});
        expect_true(tis7 != tis8);
      }

      test_that("topic indicator sequences can be added") {
        TopicIndicatorSequence tis1({1, 2, 1, 1});
        TopicIndicatorSequence tis2({1, 1, 3, 3});
        TopicIndicatorSequence tis3;

        tis3 = tis1 + tis2;

        auto counts = tis3.get_values();
        std::sort(counts.begin(), counts.end());
        check_equality(counts, {1, 2, 5});

        auto keys = tis3.get_keys();
        std::sort(keys.begin(), keys.end());
        check_equality(keys, {1, 2, 3});
      }

      test_that("topic indicator sequences can be multiplied") {
        TopicIndicatorSequence tis1({1, 2, 1, 1});
        TopicIndicatorSequence tis2({1, 1, 3, 3});
        TopicIndicatorSequence tis3;

        tis3 = tis1 * tis2;

        auto counts = tis3.get_values();
        std::sort(counts.begin(), counts.end());
        check_equality(counts, {6});

        auto keys = tis3.get_keys();
        check_equality(keys, {1});
      }
    }

  } // namespace test
} // namespace texmer
