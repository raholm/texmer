#include <catch.hpp>

#include "lexical_evaluator.h"
#include "test_helper.h"

SCENARIO("a vocabulary evaluator construction", "[constructor]") {
  GIVEN("a sentence size less than 1") {
    THEN("it should throw invalid argument exception") {
      REQUIRE_THROWS_AS(VocabularyEvaluator(0), std::invalid_argument);
    }
  }

  GIVEN("a sentence size equal 1") {
    THEN("it should not throw invalid argument exception") {
      REQUIRE_NOTHROW(VocabularyEvaluator(1));
    }
  }

  GIVEN("a sentence size greater than 1") {
    THEN("it should not throw invalid argument exception") {
      REQUIRE_NOTHROW(VocabularyEvaluator(2));
    }
  }
}

SCENARIO("a vocabulary evaluator evaluates", "[evaluate]") {
  DocumentTokenSequences document_token_sequences{
    TokenSequence({"good"}),
      TokenSequence({"what", "are"}),
      TokenSequence({"good"}),
      TokenSequence({"here", "here"})
      };

  DocumentScores document_expected_scores{3 / 4.0, 2 / 4.0, 1 / 4.0, 1 / 4.0};

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
    VocabularyEvaluator eval(2);

    THEN("the scores are as expected") {
      DocumentScores actual = eval.evaluate(document_token_sequences);

      check_equality(actual, document_expected_scores);
    }
  }

  GIVEN("a corpus of token sequences and sentence size 2") {
    VocabularyEvaluator eval(2);

    THEN("the scores are as expected") {
      CorpusScores actual = eval.evaluate(corpus_token_sequences);

      check_equality(actual, corpus_expected_scores);
    }
  }
}
