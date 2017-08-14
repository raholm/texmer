#include <catch.hpp>

#include "texttile.h"
#include "test_helper.h"

namespace texmer {
  namespace test {

    SCENARIO("textile construction", "[constructor]") {
      GIVEN("a sentence size of 0") {
        REQUIRE_THROWS_AS(TextTile(0, 1, "vocabulary", true), std::invalid_argument);
      }

      GIVEN("a block size of 0 and block method") {
        REQUIRE_THROWS_AS(TextTile(1, 0, "block", false), std::invalid_argument);
      }

      GIVEN("a block size of 0 and vocabulary method") {
        REQUIRE_NOTHROW(TextTile(1, 0, "vocabulary", false));
      }

      GIVEN("a unknown method") {
        REQUIRE_THROWS_AS(TextTile(1, 1, "unknown_method", true), std::invalid_argument);
      }
    }

    SCENARIO("a texttile segments document and corpus", "[segment]") {
      /*
        The token sequences are:
        {hello, world}, {what}, {hello}, {what, hello}, {what, what}

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

        The vocabulary scores are:
        {(2 + 1) / 4 = 0.75,
        (1 + 0) / 4 = 0.25,
        (0 + 0) / 4 = 0,
        (0 + 0) / 4 = 0,
        (0 + 0) / 4 = 0}

        The depth scores are:
        {0,
        0.5,
        0.75,
        0.75,
        0.75}

        The liberal depth cutoff score is:
        -0.1259601

        Boundary points are:
        {0, 1, 2, 3, 4}

        Adjusted boundary points are:
        {1, 3, 5, 7, 9}

        Segments are:
        {hello world, what is, that hello, what hello, what what}

        The non-liberal depth cutoff score is:
        0.03701994

        Boundary points are:
        {1, 2, 3, 4}

        Adjusted boundary points are:
        {3, 5, 7, 9}

        Segments are:
        {hello world what is, that hello, what hello, what what}
      */

      Document doc{"hello", "world",
          "what", "is",
          "that", "hello",
          "what", "hello",
          "what", "what"};
      size_t sentence_size = 2;
      size_t block_size = 2;
      Document stopwords{"is", "that"};

      Corpus corpus{doc, doc};


      GIVEN("a liberal block texttile and a document + stopwords") {
        TextTile texttile{sentence_size, block_size, "block", true};

        THEN("it segments correctly") {
          auto segments = texttile.segment(doc, stopwords);
          check_equality(segments, {"hello world", "what is", "that hello what hello", "what what"});
        }
      }

      GIVEN("a liberal block texttile and a corpus + stopwords") {
        TextTile texttile{sentence_size, block_size, "block", true};

        THEN("it segments correctly") {
          auto segments = texttile.segment(corpus, stopwords);
          check_equality(segments, {
              {"hello world", "what is", "that hello what hello", "what what"},
                {"hello world", "what is", "that hello what hello", "what what"}});
        }
      }

      GIVEN("a non-liberal block texttile and a document + stopwords") {
        TextTile texttile{sentence_size, block_size, "block", false};

        THEN("it segments correctly") {
          auto segments = texttile.segment(doc, stopwords);
          check_equality(segments, {"hello world", "what is that hello what hello what what"});
        }
      }

      GIVEN("a non-liberal block texttile and a corpus + stopwords") {
        TextTile texttile{sentence_size, block_size, "block", false};

        THEN("it segments correctly") {
          auto segments = texttile.segment(corpus, stopwords);
          check_equality(segments, {
              {"hello world", "what is that hello what hello what what"},
                {"hello world", "what is that hello what hello what what"}});
        }
      }

      GIVEN("a liberal vocabulary texttile and a document + stopwords") {
        TextTile texttile{sentence_size, block_size, "vocabulary", true};

        THEN("it segments correctly") {
          auto segments = texttile.segment(doc, stopwords);
          check_equality(segments, {"hello world", "what is", "that hello", "what hello", "what what"});
        }
      }

      GIVEN("a liberal vocabulary texttile and a corpus + stopwords") {
        TextTile texttile{sentence_size, block_size, "vocabulary", true};

        THEN("it segments correctly") {
          auto segments = texttile.segment(corpus, stopwords);
          check_equality(segments, {
              {"hello world", "what is", "that hello", "what hello", "what what"},
                {"hello world", "what is", "that hello", "what hello", "what what"}});
        }
      }

      GIVEN("a non-liberal vocabulary texttile and a document + stopwords") {
        TextTile texttile{sentence_size, block_size, "vocabulary", false};

        THEN("it segments correctly") {
          auto segments = texttile.segment(doc, stopwords);
          check_equality(segments, {"hello world what is", "that hello", "what hello", "what what"});
        }
      }

      GIVEN("a non-liberal vocabulary texttile and a corpus + stopwords") {
        TextTile texttile{sentence_size, block_size, "vocabulary", false};

        THEN("it segments correctly") {
          auto segments = texttile.segment(corpus, stopwords);
          check_equality(segments, {
              {"hello world what is", "that hello", "what hello", "what what"},
                {"hello world what is", "that hello", "what hello", "what what"}});
        }
      }
    }

  } // namespace test
} // namespace texmer
