#include <catch.hpp>

#include "type_topic_indicator_mode.h"
#include "test_helper.h"

namespace texmer {
  namespace test {

    SCENARIO("a type topic indicator mode construction", "[constructor]") {
      TypeTopicIndicatorMode::type_vector types{"hello", "world", "hello"};
      TypeTopicIndicatorMode::topic_indicator_vector topic_indicators{1, 2, 1};
      TypeTopicIndicatorMode copy(types, topic_indicators);

      REQUIRE(copy.size() == 2);

      GIVEN("two vectors of types and topic indicators") {
        TypeTopicIndicatorMode ttim{types, topic_indicators};

        THEN("the size is equal to the number of unique types") {
          REQUIRE(ttim.size() == 2);
        }
      }

      GIVEN("two vectors of types and topic indicators of mismatched sizes") {
        THEN("an invalid argument exception should be thrown") {
          REQUIRE_THROWS_AS(TypeTopicIndicatorMode({"hello", "world", "hello"}, {1, 2}), std::invalid_argument);
        }
      }

      GIVEN("a copy of topic indicator sequence") {
        TypeTopicIndicatorMode ttim{copy};

        THEN("the size is equal to the size of the copy") {
          REQUIRE(ttim.size() == copy.size());
        }
      }

      GIVEN("a copy of topic indicator sequence using move semantics") {
        TypeTopicIndicatorMode ttim{std::move(copy)};

        THEN("the size is equal to the size of the copy") {
          REQUIRE(ttim.size() == 2);
        }
      }
    }

    SCENARIO("type topic indaictor mode can be queries", "[queries]") {
      TypeTopicIndicatorMode::type_vector types{"hello", "world", "hello", "hello"};
      TypeTopicIndicatorMode::topic_indicator_vector topic_indicators{1, 2, 1, 2};
      TypeTopicIndicatorMode ttim{types, topic_indicators};

      GIVEN("a type topic indicator mode and asked if existing type is contained") {
        THEN("it should return true") {
          REQUIRE(ttim.contains("hello") == true);
        }
      }

      GIVEN("a type topic indicator mode and asked if non-existing type is contained") {
        THEN("it should return false") {
          REQUIRE(ttim.contains("hell") == false);
        }
      }

      GIVEN("a type topic indator mode") {
        THEN("it should have the correct modes") {
          REQUIRE(ttim.get_mode("hello") == 1);
          REQUIRE(ttim.get_mode("world") == 2);
        }
      }
    }

  } // namespace test
} // namespace texmer
