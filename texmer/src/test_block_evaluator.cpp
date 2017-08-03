#include <catch.hpp>

#include <cmath>

#include "lexical_evaluator.h"

static void check_equality(const DocumentScores& s1, const DocumentScores& s2) {
  REQUIRE(s1.size() == s2.size());

  for (unsigned i = 0; i < s1.size(); ++i)
    REQUIRE(s1.at(i) == Approx(s2.at(i)));
}

static void check_equality(const CorpusScores& s1, const CorpusScores& s2) {
  REQUIRE(s1.size() == s2.size());

  for (unsigned i = 0; i < s1.size(); ++i)
    check_equality(s1.at(i), s2.at(i));
}

SCENARIO("a block evaluator construction", "[constructor]") {
  GIVEN("a block size less than 1") {
    THEN("it should throw invalid argument exception") {
      REQUIRE_THROWS_AS(BlockEvaluator(0), std::invalid_argument);
    }
  }

  GIVEN("a block size equal 1") {
    THEN("it should not throw invalid argument exception") {
      REQUIRE_NOTHROW(BlockEvaluator(1));
    }
  }

  GIVEN("a block size greater than 1") {
    THEN("it should not throw invalid argument exception") {
      REQUIRE_NOTHROW(BlockEvaluator(2));
    }
  }
}

SCENARIO("a block evaluator evaluates", "[evaluate]") {
  DocumentTokenSequences document_token_sequences{
    TokenSequence({"good"}),
      TokenSequence({"what", "are", "doing", "doing"}),
      TokenSequence({"good", "good", "doing", "doing", "doing"}),
      TokenSequence({"here", "here", "good", "good", "good", "good", "what", "what", "what"})
      };

  DocumentScores document_expected_scores{0,
      (6 * 1 + 3 * 1 + 3* 2) / sqrt((1 + 1 + 1 + 4) * (36 + 9 + 4 + 9)),
      (2 * 4) / sqrt((4 + 9) * (4 + 16 + 9)),
      0};

  CorpusTokenSequences corpus_token_sequences;
  corpus_token_sequences.push_back(document_token_sequences);
  corpus_token_sequences.push_back(document_token_sequences);
  corpus_token_sequences.push_back(document_token_sequences);
  corpus_token_sequences.push_back(document_token_sequences);

  CorpusScores corpus_expected_scores;
  corpus_expected_scores.push_back(document_expected_scores);
  corpus_expected_scores.push_back(document_expected_scores);
  corpus_expected_scores.push_back(document_expected_scores);
  corpus_expected_scores.push_back(document_expected_scores);

  GIVEN("a document of token sequences and sentence size 2") {
    BlockEvaluator eval(2);

    THEN("the scores are as expected") {
      DocumentScores actual = eval.evaluate(document_token_sequences);

      check_equality(actual, document_expected_scores);
    }
  }

  GIVEN("a corpus of token sequences and sentence size 2") {
    BlockEvaluator eval(2);

    THEN("the scores are as expected") {
      CorpusScores actual = eval.evaluate(corpus_token_sequences);

      check_equality(actual, corpus_expected_scores);
    }
  }
}
