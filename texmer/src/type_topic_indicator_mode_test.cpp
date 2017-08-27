#include <testthat.h>

#include "type_topic_indicator_mode.h"
#include "test_helper.h"

namespace texmer {
  namespace test {

    context("type topic indicator mode") {
      test_that("type topic indicator mode given types and topic indicators constructs correctly") {
        TypeTopicIndicatorMode::type_vector types{"hello", "world", "hello"};
        TypeTopicIndicatorMode::topic_indicator_vector topic_indicators{1, 2, 1};

        TypeTopicIndicatorMode ttim{types, topic_indicators};
        expect_true(ttim.size() == 2);
      }

      test_that("type topic indicator mode throws an expection given types and topic indicators of mismatched sizes") {
        expect_error_as(TypeTopicIndicatorMode({"hello", "world", "hello"}, {1, 2}), std::invalid_argument);
      }

      test_that("type topic indicator mode given a copy construct correctly") {
        TypeTopicIndicatorMode::type_vector types{"hello", "world", "hello"};
        TypeTopicIndicatorMode::topic_indicator_vector topic_indicators{1, 2, 1};
        TypeTopicIndicatorMode copy(types, topic_indicators);
        expect_true(copy.size() == 2);

        TypeTopicIndicatorMode ttim{copy};
        expect_true(ttim.size() == copy.size());
      }

      test_that("type topic indicator mode given a moved copy construct correctly") {
        TypeTopicIndicatorMode::type_vector types{"hello", "world", "hello"};
        TypeTopicIndicatorMode::topic_indicator_vector topic_indicators{1, 2, 1};
        TypeTopicIndicatorMode copy(types, topic_indicators);
        expect_true(copy.size() == 2);

        TypeTopicIndicatorMode ttim{std::move(copy)};
        expect_true(ttim.size() == 2);
      }

      test_that("type topic indicator mode can be queried)") {
        TypeTopicIndicatorMode::type_vector types{"hello", "world", "hello", "hello"};
        TypeTopicIndicatorMode::topic_indicator_vector topic_indicators{1, 2, 1, 2};
        TypeTopicIndicatorMode ttim{types, topic_indicators};

        expect_true(ttim.contains("hello") == true);
        expect_true(ttim.contains("hell") == false);
        expect_true(ttim.get_mode("hello") == 1);
        expect_true(ttim.get_mode("world") == 2);
      }
    }

  } // namespace test
} // namespace texmer
