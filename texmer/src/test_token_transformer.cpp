#include <catch.hpp>

#include "token_transformer.h"
#include "test_helper.h"

SCENARIO("a token transformer requires a positive sentence size", "[constructor]") {
  GIVEN("a sentence size of 0") {
    THEN("it should throw an invalid argument error") {
      REQUIRE_THROWS_AS(TokenTransformer(0), std::invalid_argument);
    }
  }

  GIVEN("a positive sentence size") {
    THEN("it should not throw an invalid argument error") {
      REQUIRE_NOTHROW(TokenTransformer(1));
    }
  }
}

SCENARIO("a token transformer should transform tokens to token sequences", "[transform]") {
  Document doc{"is", "good", "what", "are", "that", "doing", "here", "here"};
  std::size_t sentence_size = 2;
  Document stopwords{"is", "that"};

  Corpus corpus;
  corpus.push_back(doc);
  corpus.push_back(doc);

  GIVEN("a document and sentence size of 2") {
    TokenTransformer trans{sentence_size};
    THEN("it should return token sequences") {
      auto ts = trans.transform(doc, stopwords);
      check_equality(ts, DocumentTokenSequences{
          TokenSequence({"good"}),
            TokenSequence({"what", "are"}),
            TokenSequence({"doing"}),
            TokenSequence({"here", "here"})});
    }
  }

  GIVEN("a corpus and sentence size of 2") {
    TokenTransformer trans{sentence_size};
    THEN("it should return token sequences") {
      auto ts = trans.transform(corpus, stopwords);
      check_equality(ts, CorpusTokenSequences{
          TokenSequence({"good"}),
            TokenSequence({"what", "are"}),
            TokenSequence({"doing"}),
            TokenSequence({"here", "here"})},
        {TokenSequence({"good"}),
            TokenSequence({"what", "are"}),
            TokenSequence({"doing"}),
            TokenSequence({"here", "here"})}});
  }
}
}
