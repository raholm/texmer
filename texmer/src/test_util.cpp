#include <catch.hpp>

#include "util.h"
#include "test_helper.h"

SCENARIO("util functions should work as expected", "[util]"){
  std::vector<int> vi{1, 2, 3, 4, 5};
  std::vector<double> vd{1, 2, 3, 4.5, 4.5};
  std::string s{"HellO"};

  GIVEN("a vector") {
    THEN("sum should return the vector sum") {
      REQUIRE(sum(vi) == (1 + 2 + 3 + 4 + 5));
      REQUIRE(sum(vd) == (1 + 2 + 3 + 4.5 + 4.5));
    }

    THEN("square sum should return the vector sum of squared elements") {
      REQUIRE(square_sum(vi) == (1 + 4 + 9 + 16 + 25));
      REQUIRE(square_sum(vd) == (1 + 4 + 9 + 20.25 + 20.25));
    }

    THEN("mean should return the vector mean") {
      REQUIRE(mean(vi) == (1 + 2 + 3 + 4 + 5) / 5);
      REQUIRE(mean(vd) == (1 + 2 + 3 + 4.5 + 4.5) / 5);
    }

    THEN("sd should return the vector standard deviation") {
      REQUIRE(sd(vi, false) == sqrt((pow(1 - 3, 2) + pow(2 - 3, 2) +
                                     pow(3 - 3, 2) + pow(4 - 3, 2) +
                                     pow(5 - 3, 2)) / 5));
      REQUIRE(sd(vd, false) == sqrt((pow(1 - 3, 2) + pow(2 - 3, 2) +
                                     pow(3 - 3, 2) + pow(4.5 - 3, 2) +
                                     pow(4.5 - 3, 2)) / 5));
    }

    THEN("sd (corrected) should return the corrected vector standard deviation") {
      REQUIRE(sd(vi, true) == sqrt((pow(1 - 3, 2) + pow(2 - 3, 2) +
                                    pow(3 - 3, 2) + pow(4 - 3, 2) +
                                    pow(5 - 3, 2)) / (5 - 1)));
      REQUIRE(sd(vd, true) == sqrt((pow(1 - 3, 2) + pow(2 - 3, 2) +
                                    pow(3 - 3, 2) + pow(4.5 - 3, 2) +
                                    pow(4.5 - 3, 2)) / (5 - 1)));
    }
  }

  GIVEN("a string") {
    THEN("to lower should return the same string in lowercase") {
      REQUIRE(to_lower(s) == "hello");
    }
  }
}
