#include <catch.hpp>

#include "token_sequence.h"
#include "test_helper.h"

SCENARIO("a token sequence construction", "[constructor]") {
  std::vector<TokenSequence::Token> tokens{"hello", "world", "hello"};
  TokenSequence copy{tokens};

  REQUIRE(copy.size() == 2);
  REQUIRE(copy.length() == 3);

  GIVEN("a vector of tokens") {
    TokenSequence ts{tokens};

    THEN("the size is equal to the number of unique tokens") {
      REQUIRE(ts.size() == 2);
    }

    THEN("the length is equal to the number of tokens") {
      REQUIRE(ts.length() == 3);
    }
  }

  GIVEN("a copy of token sequence") {
    TokenSequence ts{copy};

    THEN("the size is equal to the size of the copy") {
      REQUIRE(ts.size() == copy.size());
    }

    THEN("the length is equal to the length of the copy") {
      REQUIRE(ts.length() == copy.length());
    }
  }

  GIVEN("a copy of token sequence using move semantics") {
    TokenSequence ts{std::move(copy)};

    THEN("the size is equal to the size of the copy") {
      REQUIRE(ts.size() == 2);
    }

    THEN("the length is equal to the length of the copy") {
      REQUIRE(ts.length() == 3);
    }
  }
}

SCENARIO("token sequences have getters", "[getters]") {
  TokenSequence ts({"hello", "world", "hello"});

  GIVEN("a token sequence") {
    THEN("its counts is a vector corresponding to the token counts") {
      auto counts = ts.get_counts();
      std::sort(counts.begin(), counts.end());
      check_equality(counts, {1, 2});
    }

    THEN("its vocabulary contains its token") {
      auto vocabulary = ts.get_vocabulary();
      check_contains(vocabulary, {"hello", "world"});
    }
  }
}

SCENARIO("token sequences can be compared", "[comparison]") {
  GIVEN("two empty token sequences") {
    TokenSequence ts1;
    TokenSequence ts2;

    THEN("they should be equal") {
      REQUIRE(ts1 == ts2);
    }
  }

  GIVEN("two equal non-empty token sequences") {
    TokenSequence ts1({"hello", "world", "hello"});
    TokenSequence ts2({"hello", "world", "hello"});

    THEN("they should be equal regardless of token order") {
      REQUIRE(ts1 == ts2);
    }
  }

  GIVEN("two equal non-empty token sequences") {
    TokenSequence ts1({"hello", "world", "hello"});
    TokenSequence ts2({"hello", "hello", "world"});

    THEN("they should be equal") {
      REQUIRE(ts1 == ts2);
    }
  }

  GIVEN("two non-equal non-empty token sequences") {
    TokenSequence ts1({"hello", "world", "hello"});
    TokenSequence ts2({"hello", "hello"});

    THEN("they should not be equal") {
      REQUIRE(ts1 != ts2);
    }
  }
}

SCENARIO("token sequences can added, subtracted, and multiplied", "[operator]") {
  TokenSequence ts1({"hello", "world", "hello", "hello"});
  TokenSequence ts2({"hello", "hello", "what", "what"});
  TokenSequence ts3;
  Vocabulary v({"hello", "what"});

  GIVEN("two token sequences") {
    THEN("if added to each other") {
      ts3 = ts1 + ts2;

      auto counts = ts3.get_counts();
      std::sort(counts.begin(), counts.end());
      check_equality(counts, {1, 2, 5});

      auto vocabulary = ts3.get_vocabulary();
      check_contains(vocabulary, {"hello", "world", "what"});
    }
  }

  GIVEN("two token sequences") {
    THEN("if multiplied to each other") {
      ts3 = ts1 * ts2;

      auto counts = ts3.get_counts();
      std::sort(counts.begin(), counts.end());
      check_equality(counts, {6});

      auto vocabulary = ts3.get_vocabulary();
      check_contains(vocabulary, {"hello"});
      check_not_contains(vocabulary, {"world", "what"});
    }
  }

  GIVEN("one token sequence and one vocabulary") {
    THEN("if subtracted to each other") {
      ts3 = ts1 - v;

      auto counts = ts3.get_counts();
      std::sort(counts.begin(), counts.end());
      check_equality(counts, {1});

      auto vocabulary = ts3.get_vocabulary();
      check_contains(vocabulary, {"world"});
      check_not_contains(vocabulary, {"hello", "what"});
    }
  }
}
