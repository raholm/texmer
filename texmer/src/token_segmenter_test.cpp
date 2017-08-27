#include <testthat.h>

#include "token_segmenter.h"
#include "test_helper.h"

namespace texmer {
  namespace test {

    context("token segmenter") {
      test_that("token segmenter segments document given boundaries correctly") {
        Document d{"hello", "world",
            "what", "are", "you", "doing", "tonight",
            "is", "that", "a", "fishstick"};
        IntVector db{1, 6};

        auto segments = TokenSegmenter().segment(d, db);
        check_equality(segments, {"hello world",
              "what are you doing tonight",
              "is that a fishstick"});
      }

      test_that("token segmenter segments corpus given boundaries correctly") {
        Document d{"hello", "world",
            "what", "are", "you", "doing", "tonight",
            "is", "that", "a", "fishstick"};
        IntVector db{1, 6};

        Corpus c{d, d, d, d};

        IntMatrix cb;
        cb.push_back({1, 6});
        cb.push_back({2, 8});
        cb.push_back({4, 7});
        cb.push_back({0, 9});

        auto segments = TokenSegmenter().segment(c, cb);
        check_equality(segments, {
            {"hello world", "what are you doing tonight", "is that a fishstick"},
              {"hello world what", "are you doing tonight is that", "a fishstick"},
                {"hello world what are you", "doing tonight is", "that a fishstick"},
                  {"hello", "world what are you doing tonight is that a", "fishstick"}});
      }

      test_that("token segmenter throws exceptions for invalid boundaries") {
        TokenSegmenter s;
        expect_error_as(s.segment(Document{"hello", "world"}, IntVector{2}), std::out_of_range);
        expect_error_as(s.segment(Document{"hello", "world"}, IntVector{0, 1, 2}), std::out_of_range);
      }
    }

  } // namespace test
} // namespace texmer
