#include <catch.hpp>

#include "vocabulary.h"

using Document = std::vector<Vocabulary::Token>;

void check_size(const Vocabulary& vocabulary, std::size_t size) {
  REQUIRE(vocabulary.size() == size);
  REQUIRE(vocabulary.size() == vocabulary.length());
}

void check_contains(const Vocabulary& vocabulary, const Document& document) {
  for (auto const& token : document)
    REQUIRE(vocabulary.contains(token));
}

void check_not_contains(const Vocabulary& vocabulary, const Document& document) {
  for (auto const& token : document)
    REQUIRE(!vocabulary.contains(token));
}

SCENARIO("vocabularies can be constructed in various ways", "[constructor]") {
  Document d{"t1", "t2", "t1"};

  REQUIRE(d.size() == 3);

  GIVEN("an empty constructor and a document") {
    Vocabulary v;

    WHEN("asked for its size") {
      THEN("the vocabulary is empty") {
        check_size(v, 0);
      }
    }

    WHEN("asked if it contains document items") {
      THEN("it does not have them") {
        check_not_contains(v, d);
      }
    }
  }

  GIVEN("a document in constructor") {
    Vocabulary v{d};

    WHEN("asked for its size") {
      THEN("the vocabulary is the size of unique terms in the document") {
        check_size(v, 2);
      }
    }

    WHEN("asked if it contains document items")  {
      THEN("it does have them") {
        check_contains(v, d);
      }
    }
  }

  GIVEN("a copy of a vocabulary") {
    Vocabulary copy{d};
    Vocabulary v{copy};

    WHEN("asked for its size") {
      THEN("the vocabulary is the size of the copied vocabulary") {
        check_size(copy, 2);
        check_size(v, copy.size());
      }
    }

    WHEN("asked if it contains document items")  {
      THEN("it does have them") {
        check_contains(v, d);
      }
    }
  }

  GIVEN("a copy of a vocabulary using move semantics") {
    Vocabulary copy{d};
    Vocabulary v{std::move(copy)};

    WHEN("asked for its size") {
      THEN("the vocabulary is the size of the moved vocabulary") {
        check_size(v, 2);
      }
    }

    WHEN("asked if it contains document items")  {
      THEN("it does have them") {
        check_contains(v, d);
      }
    }
  }
}

SCENARIO("vocabularies can be added and subtracted", "[operators]") {
  GIVEN("a vocabulary with some tokens") {
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

    WHEN("it is subtracted by another vocabulary") {
      v1 = v1 - v2;

      THEN("the shared tokens are removed") {
        check_size(v1, 1);
        check_not_contains(v1, {"t2"});
      }
    }

    WHEN("it is subtracted by another vocabulary") {
      v1 -= v2;

      THEN("the shared tokens are removed") {
        check_size(v1, 1);
        check_not_contains(v1, {"t2"});
      }
    }

    WHEN("it is subtracted by an empty vocabulary") {
      v1 -= v3;

      THEN("nothing changes") {
        check_size(v1, 2);
        check_contains(v1, {"t1", "t2"});
        check_not_contains(v1, {"t3"});
      }
    }

    WHEN("it is added by another vocabulary") {
      v1 = v1 + v2;

      THEN("the unshared tokens are added") {
        check_size(v1, 3);
        check_contains(v1, {"t3"});
      }
    }

    WHEN("it is added by another vocabulary") {
      v1 += v2;

      THEN("the unshared tokens are added") {
        check_size(v1, 3);
        check_contains(v1, {"t3"});
      }
    }

    WHEN("it is added by an empty vocabulary") {
      v1 += v3;

      THEN("nothing changes") {
        check_size(v1, 2);
        check_contains(v1, {"t1", "t2"});
        check_not_contains(v1, {"t3"});
      }
    }
  }
}

SCENARIO("a vocabulary can be queried if it constains certain token", "[query]") {
  Document d{"t1", "t2", "t1", "t3"};
  Vocabulary v{d};

  check_size(v, 3);

  GIVEN("a non-empty vocabulary") {
    WHEN("asked if it contains an existing item") {
      THEN("it should answer it has") {
        check_contains(v, {"t1", "t2", "t3"});
      }

      WHEN("asked if it contains an non-existing item") {
        THEN("it should answet it has not") {
          check_not_contains(v, {"t0", "t4"});
        }
      }
    }
  }
}
