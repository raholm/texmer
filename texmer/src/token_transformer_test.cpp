#include <testthat.h>

#include "token_transformer.h"
#include "test_helper.h"

namespace texmer {
  namespace test {

    context("token transformer") {
      test_that("token transformer constructs correctly") {
        expect_error_as(TokenTransformer(0), std::invalid_argument);
        expect_no_error(TokenTransformer(1));
      }

      test_that("token transformer transforms document correctly") {
        Document doc{"is", "good", "what", "are", "that", "doing", "here", "here"};
        size_t sentence_size = 2;
        Document stopwords{"is", "that"};

        TokenTransformer trans{sentence_size};
        auto ts = trans.transform(doc, stopwords);
        check_equality(ts, DocumentTokenSequences{
            TokenSequence({"good"}),
              TokenSequence({"what", "are"}),
              TokenSequence({"doing"}),
              TokenSequence({"here", "here"})});
      }

      test_that("token transformer transforms corpus correctly") {
        Document doc{"is", "good", "what", "are", "that", "doing", "here", "here"};
        Corpus corpus{doc, doc};
        size_t sentence_size = 2;
        Document stopwords{"is", "that"};

        TokenTransformer trans{sentence_size};
        auto ts = trans.transform(corpus, stopwords);
        check_equality(ts, CorpusTokenSequences{{
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

  } // namespace test
} // namespace texmer
