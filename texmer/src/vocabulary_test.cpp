#include <testthat.h>

#include "vocabulary.h"
#include "test_helper.h"
#include "def.h"

namespace texmer {
  namespace test {

    context("vocabulary") {
      test_that("empty vocabulary is constructed correctly") {
        Document d{"t1", "t2", "t1"};
        expect_true(d.size() == 3);

        Vocabulary v;
        check_size(v, 0);
        check_not_contains(v, d);
      }

      test_that("vocabulary given document is constructed correctly") {
        Document d{"t1", "t2", "t1"};
        expect_true(d.size() == 3);

        Vocabulary v{d};
        check_size(v, 2);
        check_contains(v, d);
      }

      test_that("vocabulary given a copy of vocabulary is constructed correctly") {
        Document d{"t1", "t2", "t1"};
        expect_true(d.size() == 3);

        Vocabulary copy{d};
        Vocabulary v{copy};
        check_size(copy, 2);
        check_size(v, copy.size());
            check_contains(v, d);
      }

      test_that("vocabulary given a moved copy of vocabulary is constructed correctly") {
        Document d{"t1", "t2", "t1"};
        expect_true(d.size() == 3);

        Vocabulary copy{d};
        Vocabulary v{std::move(copy)};
        check_size(v, 2);
        check_contains(v, d);
      }

      test_that("vocabulary can be subtracted by other vocabulary") {
        Document d1{"t1", "t2", "t1"};
        Vocabulary v1{d1};

        Document d2{"t3", "t2"};
        Vocabulary v2{d2};

        Vocabulary v3;

        check_size(v1, 2);
        check_size(v2, 2);
        check_size(v3, 0);

        check_contains(v1, {"t1", "t2"});
        check_contains(v2, {"t3", "t2"});

        auto a1 = v1 - v2;
        check_size(a1, 1);
        check_not_contains(a1, {"t2"});

        auto a2 = v1;
        a2 -= v2;
        check_size(a2, 1);
        check_not_contains(a2, {"t2"});

        auto a3 = v1;
        a3 -= v3;
        check_size(v1, 2);
        check_contains(v1, {"t1", "t2"});
        check_not_contains(v1, {"t3"});
      }

      test_that("vocabulary can be added by other vocabulary") {
        Document d1{"t1", "t2", "t1"};
        Vocabulary v1{d1};

        Document d2{"t3", "t2"};
        Vocabulary v2{d2};

        Vocabulary v3;

        check_size(v1, 2);
        check_size(v2, 2);
        check_size(v3, 0);

        check_contains(v1, {"t1", "t2"});
        check_contains(v2, {"t3", "t2"});

        auto a1 = v1 + v2;
        check_size(a1, 3);
        check_contains(a1, {"t3"});

        auto a2 = v1;
        a2 += v2;
        check_size(a2, 3);
        check_contains(a2, {"t3"});

        auto a3 = v1;
        a3 += v3;
        check_size(a3, 2);
        check_contains(a3, {"t1", "t2"});
        check_not_contains(a3, {"t3"});
      }

      test_that("vocabulary can be queried") {
        Document d{"t1", "t2", "t1", "t3"};
        Vocabulary v{d};

        check_size(v, 3);
        check_contains(v, {"t1", "t2", "t3"});
        check_not_contains(v, {"t0", "t4"});
      }
    }

  } // namespace test
} // namespace texmer
