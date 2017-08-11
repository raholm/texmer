#include <catch.hpp>

#include "topic_indicator_sequence.h"
#include "test_helper.h"

SCENARIO("a topic indicator sequence construction", "[constructor]") {
  std::vector<TopicIndicatorSequence::topic_indicator> topic_indicators{1, 2, 1};
  TopicIndicatorSequence copy(topic_indicators);

  REQUIRE(copy.size() == 2);
  REQUIRE(copy.length() == 3);

  GIVEN("a vector of topic indicators") {
    TopicIndicatorSequence tis{topic_indicators};

    THEN("the size is equal to the number of unique tokens") {
      REQUIRE(tis.size() == 2);
    }

    THEN("the length is equal to the number of tokens") {
      REQUIRE(tis.length() == 3);
    }
  }

  GIVEN("a copy of topic indicator sequence") {
    TopicIndicatorSequence tis{copy};

    THEN("the size is equal to the size of the copy") {
      REQUIRE(tis.size() == copy.size());
    }

    THEN("the length is equal to the length of the copy") {
      REQUIRE(tis.length() == copy.length());
    }
  }

  GIVEN("a copy of topic indicator sequence using move semantics") {
    TopicIndicatorSequence tis{std::move(copy)};

    THEN("the size is equal to the size of the copy") {
      REQUIRE(tis.size() == 2);
    }

    THEN("the length is equal to the length of the copy") {
      REQUIRE(tis.length() == 3);
    }
  }
}

SCENARIO("topic indicator sequences have getters", "[getters]") {
  TopicIndicatorSequence tis({1, 2, 1});

  GIVEN("a topic indicator sequence") {
    THEN("its counts is a vector corresponding to the topic indicator counts") {
      auto counts = tis.get_values();
      std::sort(counts.begin(), counts.end());
      check_equality(counts, {1, 2});
    }

    THEN("its keys contains its topic indicators") {
      auto keys = tis.get_keys();
      std::sort(keys.begin(), keys.end());
      check_equality(keys, {1, 2});
    }
  }
}

SCENARIO("topic indicator sequences can be compared", "[comparison]") {
  GIVEN("two empty topic indicator sequences") {
    TopicIndicatorSequence tis1;
    TopicIndicatorSequence tis2;

    THEN("they should be equal") {
      REQUIRE(tis1 == tis2);
    }
  }

  GIVEN("two equal non-empty topic indicator sequences") {
    TopicIndicatorSequence tis1({1, 2, 1});
    TopicIndicatorSequence tis2({1, 2, 1});

    THEN("they should be equal") {
      REQUIRE(tis1 == tis2);
    }
  }

  GIVEN("two equal non-empty topic indicator sequences") {
    TopicIndicatorSequence tis1({1, 2, 1});
    TopicIndicatorSequence tis2({1, 1, 2});

    THEN("they should be equal regardless of topic indicator order") {
      REQUIRE(tis1 == tis2);
    }
  }

  GIVEN("two non-equal non-empty topic indicator sequences") {
    TopicIndicatorSequence tis1({1, 2, 1});
    TopicIndicatorSequence tis2({1, 1});

    THEN("they should not be equal") {
      REQUIRE(tis1 != tis2);
    }
  }
}

SCENARIO("topic indicator sequences can added, subtracted, and multiplied", "[operator]") {
  TopicIndicatorSequence tis1({1, 2, 1, 1});
  TopicIndicatorSequence tis2({1, 1, 3, 3});
  TopicIndicatorSequence tis3;

  GIVEN("two topic indicator sequences") {
    THEN("if added to each other") {
      tis3 = tis1 + tis2;

      auto counts = tis3.get_values();
      std::sort(counts.begin(), counts.end());
      check_equality(counts, {1, 2, 5});

      auto keys = tis3.get_keys();
      std::sort(keys.begin(), keys.end());
      check_equality(keys, {1, 2, 3});
    }
  }

  GIVEN("two topic indicator sequences") {
    THEN("if multiplied to each other") {
      tis3 = tis1 * tis2;

      auto counts = tis3.get_values();
      std::sort(counts.begin(), counts.end());
      check_equality(counts, {6});

      auto keys = tis3.get_keys();
      check_equality(keys, {1});
    }
  }
}
