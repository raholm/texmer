#include <testthat.h>

#include "topictile.h"
#include "test_helper.h"

namespace texmer {
  namespace test {

    context("topictile") {
      // test_that("construction works properly") {
      //   expect_error_as(TextTile(0, 1, "vocabulary", true), std::invalid_argument);
      //   expect_error_as(TextTile(1, 0, "block", false), std::invalid_argument);
      //   expect_no_error(TextTile(1, 0, "vocabulary", false));
      //   expect_error_as(TextTile(1, 1, "unknown_method", true), std::invalid_argument);
      // }

      /*
        The token sequences are:
        {hello, world}, {what}, {hello}, {what, hello}, {what, what}

        The topic indicator sequences are:
        {0, 1}, {2}, {0}, {2, 0}, {2, 2}

        The block scores are:
        {0,
        (1 * 2 + 1 * 1) / sqrt((1^2 + 1^2 + 1^2) * (2^2 + 1^2)) = 0.7745967,
        (3 * 1 + 1 * 1) / sqrt((1^2 + 1^2) * (3^2 + 1^2)) = 0.8944272,
        (2 * 1) / sqrt((1^1 + 1^1) * (2^2)) = 0.7071068,
        0}

        The depth scores are:
        {0.8944272,
        0.8944272 - 0.7745967 = 0.1198305,
        0,
        0.8944272 - 0.7745967 = 0.1198305,
        0.7071068}

        The liberal depth cutoff score is:
        0.03624987

        Boundary points are:
        {0, 1, 3, 4}

        Adjusted boundary points are:
        {1, 3, 7, 9}

        Segments are:
        {hello world, what is, that hello what hello, what what}

        the non-liberal depth cutoff score is:
        0.255738

        Boundary points are:
        {0, 4}

        Adjusted boundary points are:
        {1, 9}

        Segments are:
        {hello world, what is that hello what hello what what}
      */

      test_that("topictile liberal block method segments document correctly") {
        Document doc{"hello", "world",
            "what", "is",
            "that", "hello",
            "what", "hello",
            "what", "what"};
        size_t sentence_size = 2;
        size_t block_size = 2;
        Document stopwords{"is", "that"};
        TypeTopicIndicatorMode modes{{"hello", "world", "what"}, {0, 1, 2}};

        TopicTile topictile{sentence_size, block_size, modes, 0, true};
        auto segments = topictile.segment(doc, stopwords);
        check_equality(segments, {"hello world", "what is", "that hello what hello", "what what"});
      }

      // test_that("topictile liberal block method segments corpus correctly") {
      //   Document doc{"hello", "world",
      //       "what", "is",
      //       "that", "hello",
      //       "what", "hello",
      //       "what", "what"};
      //   size_t sentence_size = 2;
      //   size_t block_size = 2;
      //   Document stopwords{"is", "that"};
      //   Corpus corpus{doc, doc};

      //   Topictile topictile{sentence_size, block_size, "block", true};
      //   auto segments = topictile.segment(corpus, stopwords);
      //   check_equality(segments, {
      //       {"hello world", "what is", "that hello what hello", "what what"},
      //         {"hello world", "what is", "that hello what hello", "what what"}});
      // }

      // test_that("topictile non-liberal block method segments document correctly") {
      //   Document doc{"hello", "world",
      //       "what", "is",
      //       "that", "hello",
      //       "what", "hello",
      //       "what", "what"};
      //   size_t sentence_size = 2;
      //   size_t block_size = 2;
      //   Document stopwords{"is", "that"};

      //   Topictile topictile{sentence_size, block_size, "block", false};
      //   auto segments = topictile.segment(doc, stopwords);
      //   check_equality(segments, {"hello world", "what is that hello what hello what what"});
      // }

      // test_that("topictile non-liberal block method segments corpus correctly") {
      //   Document doc{"hello", "world",
      //       "what", "is",
      //       "that", "hello",
      //       "what", "hello",
      //       "what", "what"};
      //   size_t sentence_size = 2;
      //   size_t block_size = 2;
      //   Document stopwords{"is", "that"};
      //   Corpus corpus{doc, doc};

      //   Topictile topictile{sentence_size, block_size, "block", false};
      //   auto segments = topictile.segment(corpus, stopwords);
      //   check_equality(segments, {
      //       {"hello world", "what is that hello what hello what what"},
      //         {"hello world", "what is that hello what hello what what"}});
      // }
    }

  } // namespace test
} // namespace texmer
