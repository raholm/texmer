#include <catch.hpp>

#include "boundary_identifier.h"
#include "test_helper.h"

namespace texmer {
  namespace test {

    SCENARIO("a texttile boundary identifier gets scores to find boundaries", "[boundary]") {
      /*
        The depth scores are {0, 4, 0, 7, 7, 2, 0, 2}
        The liberal depth cutoff score is 1.417749
        the non-liberal depth cutoff score is 2.333875
       */

      DoubleVector vscores{5, 2, 3, 1, 1, 4, 6, 4};

      DoubleMatrix mscores;
      mscores.push_back(vscores);
      mscores.push_back(vscores);
      mscores.push_back(vscores);
      mscores.push_back(vscores);

      GIVEN("a liberal texttile boundary identifier and a vector of scores") {
        TextTileBoundaryIdentifier identifier{true};

        THEN("it gets the right boundaries") {
          auto boundaries = identifier.get_boundaries(vscores);
          check_equality(boundaries, {1, 3, 4, 5, 7});
        }
      }

      GIVEN("a liberal texttile boundary identifier and a matrix of scores") {
        TextTileBoundaryIdentifier identifier{true};

        THEN("it gets the right boundaries") {
          auto boundaries = identifier.get_boundaries(mscores);
          check_equality(boundaries, {{1, 3, 4, 5, 7},
                {1, 3, 4, 5, 7}, {1, 3, 4, 5, 7}, {1, 3, 4, 5, 7}});
        }
      }

      GIVEN("a non-liberal texttile boundary identifier and a vector of scores") {
        TextTileBoundaryIdentifier identifier{false};

        THEN("it gets the right boundaries") {
          auto boundaries = identifier.get_boundaries(vscores);
          check_equality(boundaries, {1, 3, 4});
        }
      }

      GIVEN("a non-liberal texttile boundary identifier and a matrix of scores") {
        TextTileBoundaryIdentifier identifier{false};

        THEN("it gets the right boundaries") {
          auto boundaries = identifier.get_boundaries(mscores);
          check_equality(boundaries, {{1, 3, 4}, {1, 3, 4}, {1, 3, 4}, {1, 3, 4}});
        }
      }
    }

    SCENARIO("a topictile boundary identifier gets scores to find boundaries", "[boundary]") {
      /*
        The depth scores are {0, 2, 0, 3,5, 3.5, 1, 0, 1}
        The liberal depth cutoff score is 1.417749
        the non-liberal depth cutoff score is 2.333875
      */

      DoubleVector vscores{5, 2, 3, 1, 1, 4, 6, 4};

      DoubleMatrix mscores;
      mscores.push_back(vscores);
      mscores.push_back(vscores);
      mscores.push_back(vscores);
      mscores.push_back(vscores);

      GIVEN("a liberal topictile boundary identifier and a vector of scores") {
        TopicTileBoundaryIdentifier identifier{-1, true};

        THEN("it gets the right boundaries") {
          auto boundaries = identifier.get_boundaries(vscores);
          check_equality(boundaries, {1, 3, 4});
        }
      }

      GIVEN("a liberal topictile boundary identifier and a matrix of scores") {
        TopicTileBoundaryIdentifier identifier{-1, true};

        THEN("it gets the right boundaries") {
          auto boundaries = identifier.get_boundaries(mscores);
          check_equality(boundaries, {
              {1, 3, 4}, {1, 3, 4}, {1, 3, 4}, {1, 3, 4}
            });
        }
      }

      GIVEN("a non-liberal topictile boundary identifier and a vector of scores") {
        TopicTileBoundaryIdentifier identifier{-1, false};

        THEN("it gets the right boundaries") {
          auto boundaries = identifier.get_boundaries(vscores);
          check_equality(boundaries, {3, 4});
        }
      }

      GIVEN("a non-liberal topictile boundary identifier and a matrix of scores") {
        TopicTileBoundaryIdentifier identifier{-1, false};

        THEN("it gets the right boundaries") {
          auto boundaries = identifier.get_boundaries(mscores);
          check_equality(boundaries, {{3, 4}, {3, 4}, {3, 4}, {3, 4}});
        }
      }

      GIVEN("a topictile boundary identifier with fixed number of segments and a vector of scores") {
        TopicTileBoundaryIdentifier identifier{2, false};

        THEN("it gets the right boundaries") {
          auto boundaries = identifier.get_boundaries(vscores);
          check_equality(boundaries, {3, 4});
        }
      }

      GIVEN("a topictile boundary identifier with fixed number of segments and a matrix of scores") {
        TopicTileBoundaryIdentifier identifier{2, false};

        THEN("it gets the right boundaries") {
          auto boundaries = identifier.get_boundaries(mscores);
          check_equality(boundaries, {{3, 4}, {3, 4}, {3, 4}, {3, 4}});
        }
      }
    }

  } // namespace test
} // namespace texmer
