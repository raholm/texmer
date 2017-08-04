#include <catch.hpp>

#include "token_segmenter.h"
#include "test_helper.h"

SCENARIO("a token segmenter can segment text") {
  Document d{"hello", "world",
      "what", "are", "you", "doing", "tonight",
      "is", "that", "a", "fishstick"};
  IntVector db{1, 6};

  Corpus c;
  c.push_back(d);
  c.push_back(d);
  c.push_back(d);
  c.push_back(d);

  IntMatrix cb;
  cb.push_back({1, 6});
  cb.push_back({2, 8});
  cb.push_back({4, 7});
  cb.push_back({0, 9});

  GIVEN("a document of tokens and a vector of boundaries") {
    THEN("it returns segments according to the boundaries") {
      auto segments = TokenSegmenter().segment(d, db);
      check_equality(segments, {"hello world",
            "what are you doing tonight",
            "is that a fishstick"});
    }
  }

  GIVEN("a corpus of tokens and a matrix of boundaries") {
    THEN("it returns segments according to the boundaries") {
      auto segments = TokenSegmenter().segment(c, cb);
      check_equality(segments, {
          {"hello world", "what are you doing tonight", "is that a fishstick"},
            {"hello world what", "are you doing tonight is that", "a fishstick"},
              {"hello world what are you", "doing tonight is", "that a fishstick"},
                {"hello", "world what are you doing tonight is that a", "fishstick"}});
    }
  }

  GIVEN("a document of tokens and a vector of boundaries") {
    TokenSegmenter s;

    THEN("if a boundary point is out of range") {
      REQUIRE_THROWS_AS(s.segment(Document{"hello", "world"}, IntVector{2}), std::out_of_range);
    }

    THEN("if a boundary point is out of range") {
      REQUIRE_THROWS_AS(s.segment(Document{"hello", "world"}, IntVector{0, 1, 2}), std::out_of_range);
    }
  }
}
