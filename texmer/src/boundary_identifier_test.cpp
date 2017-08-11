#include <catch.hpp>

#include "boundary_identifier.h"
#include "test_helper.h"

namespace texmer {
  namespace test {

    SCENARIO("a texttile boundary identifier gets scores to find boundaries", "[boundary]") {
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

  } // namespace test
} // namespace texmer
