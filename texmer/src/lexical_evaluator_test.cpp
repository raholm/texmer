#include <testthat.h>

#include <cmath>

#include "lexical_evaluator.h"
#include "test_helper.h"

namespace texmer {
  namespace test {

    context("block evaluator") {
      test_that("block evaluator constructs correctly") {
          expect_error_as(TextTileBlockEvaluator(0), std::invalid_argument);
          expect_no_error(TextTileBlockEvaluator(1));
          expect_no_error(TextTileBlockEvaluator(2));
      }

      test_that("block evaluator evaluates document correctly") {
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

        TextTileBlockEvaluator eval(2);
        DocumentScores actual = eval.evaluate(document_token_sequences);
        check_equality(actual, document_expected_scores);
      }

      test_that("block evaluator evaluates corpus correctly") {
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

        CorpusTokenSequences corpus_token_sequences{document_token_sequences,
            document_token_sequences, document_token_sequences, document_token_sequences};

        CorpusScores corpus_expected_scores{document_expected_scores,
            document_expected_scores, document_expected_scores, document_expected_scores};

        TextTileBlockEvaluator eval(2);
        CorpusScores actual = eval.evaluate(corpus_token_sequences);
        check_equality(actual, corpus_expected_scores);
      }
    }

    context("vocabulary evaluator") {
      test_that("vocabulary evaluator constructs correctly") {
          expect_error_as(TextTileVocabularyEvaluator(0), std::invalid_argument);
          expect_no_error(TextTileVocabularyEvaluator(1));
          expect_no_error(TextTileVocabularyEvaluator(2));
      }

      test_that("vocabluary evaluator evaluates document correctly") {
        DocumentSequences<TokenSequence> document_token_sequences{
          TokenSequence({"good"}),
            TokenSequence({"what", "are"}),
            TokenSequence({"good"}),
            TokenSequence({"here", "here"})
            };

        DocumentScores document_expected_scores{3 / 4.0, 2 / 4.0, 1 / 4.0, 1 / 4.0};

        TextTileVocabularyEvaluator eval(2);
          DocumentScores actual = eval.evaluate(document_token_sequences);
          check_equality(actual, document_expected_scores);
      }

      test_that("vocabulary evaluator evaluates corpus correctly") {
        DocumentSequences<TokenSequence> document_token_sequences{
          TokenSequence({"good"}),
            TokenSequence({"what", "are"}),
            TokenSequence({"good"}),
            TokenSequence({"here", "here"})
            };

        DocumentScores document_expected_scores{3 / 4.0, 2 / 4.0, 1 / 4.0, 1 / 4.0};

        CorpusSequences<TokenSequence> corpus_token_sequences{document_token_sequences,
            document_token_sequences, document_token_sequences, document_token_sequences};

        CorpusScores corpus_expected_scores{document_expected_scores,
            document_expected_scores, document_expected_scores, document_expected_scores};

        TextTileVocabularyEvaluator eval(2);
        CorpusScores actual = eval.evaluate(corpus_token_sequences);
        check_equality(actual, corpus_expected_scores);
      }
    }

  } // namespace test
} // namespace texmer
