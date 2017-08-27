#include <testthat.h>

#include "util.h"
#include "test_helper.h"
#include "def.h"

namespace texmer {
  namespace test {

    context("utility functions") {
      test_that("sum of vector is correct") {
        IntVector vi{1, 2, 3, 4, 5};
        DoubleVector vd{1, 2, 3, 4.5, 4.5};

        expect_true(sum(vi) == (1 + 2 + 3 + 4 + 5));
        expect_true(sum(vd) == (1 + 2 + 3 + 4.5 + 4.5));
      }

      test_that("square_sum of vector is correct") {
        IntVector vi{1, 2, 3, 4, 5};
        DoubleVector vd{1, 2, 3, 4.5, 4.5};

        expect_true(square_sum(vi) == (1 + 4 + 9 + 16 + 25));
        expect_true(square_sum(vd) == (1 + 4 + 9 + 20.25 + 20.25));
      }

      test_that("mean of vector is correct") {
        IntVector vi{1, 2, 3, 4, 5};
        DoubleVector vd{1, 2, 3, 4.5, 4.5};

        expect_true(mean(vi) == (1 + 2 + 3 + 4 + 5) / 5);
        expect_true(mean(vd) == (1 + 2 + 3 + 4.5 + 4.5) / 5);
      }

      test_that("sd of vector is correct") {
        IntVector vi{1, 2, 3, 4, 5};
        DoubleVector vd{1, 2, 3, 4.5, 4.5};

        expect_true(sd(vi, false) == sqrt((pow(1 - 3, 2) + pow(2 - 3, 2) +
                                           pow(3 - 3, 2) + pow(4 - 3, 2) +
                                           pow(5 - 3, 2)) / 5));
        expect_true(sd(vd, false) == sqrt((pow(1 - 3, 2) + pow(2 - 3, 2) +
                                           pow(3 - 3, 2) + pow(4.5 - 3, 2) +
                                           pow(4.5 - 3, 2)) / 5));
      }

      test_that("corrected sd of vector is correct") {
        IntVector vi{1, 2, 3, 4, 5};
        DoubleVector vd{1, 2, 3, 4.5, 4.5};

        expect_true(sd(vi, true) == sqrt((pow(1 - 3, 2) + pow(2 - 3, 2) +
                                          pow(3 - 3, 2) + pow(4 - 3, 2) +
                                          pow(5 - 3, 2)) / (5 - 1)));
        expect_true(sd(vd, true) == sqrt((pow(1 - 3, 2) + pow(2 - 3, 2) +
                                          pow(3 - 3, 2) + pow(4.5 - 3, 2) +
                                          pow(4.5 - 3, 2)) / (5 - 1)));
      }

      test_that("to_lower of string is correct") {
        String s{"HellO"};

        expect_true(to_lower(s) == "hello");
      }
    }

  } // namespace test
} // namespace texmer
