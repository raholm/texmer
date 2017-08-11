#include <catch.hpp>

#include <cmath>

#include "lexical_evaluator.h"
#include "test_helper.h"

namespace texmer {
  namespace test {

    SCENARIO("a block evaluator construction", "[constructor]") {
      GIVEN("a block size less than 1") {
        THEN("it should throw invalid argument exception") {
          REQUIRE_THROWS_AS(TextTileBlockEvaluator(0), std::invalid_argument);
        }
      }

      GIVEN("a block size equal 1") {
        THEN("it should not throw invalid argument exception") {
          REQUIRE_NOTHROW(TextTileBlockEvaluator(1));
        }
      }

      GIVEN("a block size greater than 1") {
        THEN("it should not throw invalid argument exception") {
          REQUIRE_NOTHROW(TextTileBlockEvaluator(2));
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

      DocumentScores document_expected_scores{
        0,
          (6 * 1 + 3 * 1 + 3 * 2) / sqrt((1 + 1 + 1 + 4) * (36 + 9 + 4 + 9)),
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
        TextTileBlockEvaluator eval(2);

        THEN("the scores are as expected") {
          DocumentScores actual = eval.evaluate(document_token_sequences);

          check_equality(actual, document_expected_scores);
        }
      }

      GIVEN("a corpus of token sequences and sentence size 2") {
        TextTileBlockEvaluator eval(2);

        THEN("the scores are as expected") {
          CorpusScores actual = eval.evaluate(corpus_token_sequences);

          check_equality(actual, corpus_expected_scores);
        }
      }
    }

    SCENARIO("a vocabulary evaluator construction", "[constructor]") {
      GIVEN("a sentence size less than 1") {
        THEN("it should throw invalid argument exception") {
          REQUIRE_THROWS_AS(TextTileVocabularyEvaluator(0), std::invalid_argument);
        }
      }

      GIVEN("a sentence size equal 1") {
        THEN("it should not throw invalid argument exception") {
          REQUIRE_NOTHROW(TextTileVocabularyEvaluator(1));
        }
      }

      GIVEN("a sentence size greater than 1") {
        THEN("it should not throw invalid argument exception") {
          REQUIRE_NOTHROW(TextTileVocabularyEvaluator(2));
        }
      }
    }

    SCENARIO("a vocabulary evaluator evaluates", "[evaluate]") {
      DocumentSequences<TokenSequence> document_token_sequences{
        TokenSequence({"good"}),
          TokenSequence({"what", "are"}),
          TokenSequence({"good"}),
          TokenSequence({"here", "here"})
          };

      DocumentScores document_expected_scores{3 / 4.0, 2 / 4.0, 1 / 4.0, 1 / 4.0};

      CorpusSequences<TokenSequence> corpus_token_sequences;
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
        TextTileVocabularyEvaluator eval(2);

        THEN("the scores are as expected") {
          DocumentScores actual = eval.evaluate(document_token_sequences);

          check_equality(actual, document_expected_scores);
        }
      }

      GIVEN("a corpus of token sequences and sentence size 2") {
        TextTileVocabularyEvaluator eval(2);

        THEN("the scores are as expected") {
          CorpusScores actual = eval.evaluate(corpus_token_sequences);

          check_equality(actual, corpus_expected_scores);
        }
      }
    }

  } // namespace test
} // namespace texmer
