#include <testthat.h>

#include "boundary_identifier.h"
#include "test_helper.h"

namespace texmer {
  namespace test {
    context("texttile boundary identifier") {
      /*
        The depth scores are {0, 4, 0, 7, 7, 2, 0, 2}
        The liberal depth cutoff score is 1.417749
        the non-liberal depth cutoff score is 2.333875
      */

      test_that("liberal texttile boundary identifier finds correct boundaries in a vector of scores") {
        DoubleVector vscores{5, 2, 3, 1, 1, 4, 6, 4};

        TextTileBoundaryIdentifier identifier{true};
        auto boundaries = identifier.get_boundaries(vscores);
        check_equality(boundaries, {1, 3, 4, 5, 7});
      }

      test_that("liberal texttile boundary identifier finds correct boundaries in a matrix of scores") {
        DoubleVector vscores{5, 2, 3, 1, 1, 4, 6, 4};
        DoubleMatrix mscores{vscores, vscores, vscores, vscores};

        TextTileBoundaryIdentifier identifier{true};
        auto boundaries = identifier.get_boundaries(mscores);
        check_equality(boundaries, {{1, 3, 4, 5, 7},
              {1, 3, 4, 5, 7}, {1, 3, 4, 5, 7}, {1, 3, 4, 5, 7}});
      }

      test_that("non-liberal texttile boundary identifier finds correct boundaries in a vector of scores") {
        DoubleVector vscores{5, 2, 3, 1, 1, 4, 6, 4};

        TextTileBoundaryIdentifier identifier{false};
        auto boundaries = identifier.get_boundaries(vscores);
        check_equality(boundaries, {1, 3, 4});
      }

      test_that("non-liberal texttile boundary identifier finds correct boundaries in a matrix of scores") {
        DoubleVector vscores{5, 2, 3, 1, 1, 4, 6, 4};
        DoubleMatrix mscores{vscores, vscores, vscores, vscores};

        TextTileBoundaryIdentifier identifier{false};
        auto boundaries = identifier.get_boundaries(mscores);
        check_equality(boundaries, {{1, 3, 4}, {1, 3, 4}, {1, 3, 4}, {1, 3, 4}});
      }
    }

    context("topictile boundary identifier") {
      /*
        The depth scores are {0, 2, 0, 3,5, 3.5, 1, 0, 1}
        The liberal depth cutoff score is 1.417749
        the non-liberal depth cutoff score is 2.333875
      */

      test_that("liberal topictile boundary identifier finds correct boundaries in a vector of scores") {
        DoubleVector vscores{5, 2, 3, 1, 1, 4, 6, 4};

        TopicTileBoundaryIdentifier identifier{-1, true};
        auto boundaries = identifier.get_boundaries(vscores);
        check_equality(boundaries, {1, 3, 4});
      }

      test_that("liberal topictile boundary identifier finds correct boundaries in a matrix of scores") {
        DoubleVector vscores{5, 2, 3, 1, 1, 4, 6, 4};
        DoubleMatrix mscores{vscores, vscores, vscores, vscores};

        TopicTileBoundaryIdentifier identifier{-1, true};
        auto boundaries = identifier.get_boundaries(mscores);
        check_equality(boundaries, {
            {1, 3, 4}, {1, 3, 4}, {1, 3, 4}, {1, 3, 4}
          });
      }

      test_that("non-liberal topictile boundary identifier finds correct boundaries in a vector of scores") {
        DoubleVector vscores{5, 2, 3, 1, 1, 4, 6, 4};

        TopicTileBoundaryIdentifier identifier{-1, false};
        auto boundaries = identifier.get_boundaries(vscores);
        check_equality(boundaries, {3, 4});
      }

      test_that("non-liberal topictile boundary identifier finds correct boundaries in a matrix of scores") {
        DoubleVector vscores{5, 2, 3, 1, 1, 4, 6, 4};
        DoubleMatrix mscores{vscores, vscores, vscores, vscores};

        TopicTileBoundaryIdentifier identifier{-1, false};
        auto boundaries = identifier.get_boundaries(mscores);
        check_equality(boundaries, {{3, 4}, {3, 4}, {3, 4}, {3, 4}});
      }

      test_that("topictile boundary identifier with fixed number of segments finds correct boundaries in a vector of scores") {
        DoubleVector vscores{5, 2, 3, 1, 1, 4, 6, 4};

        TopicTileBoundaryIdentifier identifier{2, false};
        auto boundaries = identifier.get_boundaries(vscores);
        check_equality(boundaries, {3, 4});
      }

      test_that("topictile boundary identifier with fixed number of segments finds correct boundaries in a matrix of scores") {
        DoubleVector vscores{5, 2, 3, 1, 1, 4, 6, 4};
        DoubleMatrix mscores{vscores, vscores, vscores, vscores};

        TopicTileBoundaryIdentifier identifier{2, false};
        auto boundaries = identifier.get_boundaries(mscores);
        check_equality(boundaries, {{3, 4}, {3, 4}, {3, 4}, {3, 4}});
      }
    }

  } // namespace test
} // namespace texmer
