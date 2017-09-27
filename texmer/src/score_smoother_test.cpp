#include <testthat.h>

#include "score_smoother.h"
#include "test_helper.h"

namespace texmer {
  namespace test {

    context("average score smoother") {
      test_that("construction works properly") {
        expect_error_as(AverageScoreSmoother(0, 1), std::invalid_argument);
        expect_error_as(AverageScoreSmoother(0, 3), std::invalid_argument);
        expect_no_error(AverageScoreSmoother(0, 0));
        expect_no_error(AverageScoreSmoother(0, 2));
        expect_no_error(AverageScoreSmoother(0, 4));
      }

      test_that("smoothing is done correctly on vector of scores") {
        DocumentScores dscores{2, 4, 6, 4, 2};

        check_equality(AverageScoreSmoother(0, 2).smooth(dscores), dscores);
        check_equality(AverageScoreSmoother(1, 2).smooth(dscores), {3, 4, 4.666667, 4, 3});
        check_equality(AverageScoreSmoother(2, 2).smooth(dscores), {3.5, 3.888889, 4.222222, 3.88889, 3.5});
        check_equality(AverageScoreSmoother(1, 4).smooth(dscores), {4, 4, 3.6, 4, 4});
        check_equality(AverageScoreSmoother(2, 4).smooth(dscores), {3.866667, 3.9, 3.92, 3.9, 3.866667});
      }
    }

  } // namespace test
} // namespace texmer
