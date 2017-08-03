#include <catch.hpp>

#include "vocabulary.h"

using Document = std::vector<Vocabulary::Token>;

SCENARIO("vocabularies can be constructed in various ways", "[constructor]") {
  Document d{"t1", "t2", "t1"};

  REQUIRE(d.size() == 3);

  GIVEN("an empty constructor and a document") {
    Vocabulary v;

    WHEN("asked for its size") {
      THEN("the vocabulary is empty") {
        REQUIRE(v.size() == 0);
        REQUIRE(v.size() == v.length());
      }
    }

    WHEN("asked if it contains document items") {
      THEN("it does not have them") {
        for (auto const &t : d)
          REQUIRE(!v.is_in(t));
      }
    }
  }

  GIVEN("a document in constructor") {
    Vocabulary v{d};

    WHEN("asked for its size") {
      THEN("the vocabulary is the size of unique terms in the document") {
        REQUIRE(v.size() == 2);
        REQUIRE(v.size() == v.length());
      }
    }

    WHEN("asked if it contains document items")  {
      THEN("it does have them") {
        for (auto const& t : d)
          REQUIRE(v.is_in(t));
      }
    }
  }

  GIVEN("a copy of a vocabulary") {
    Vocabulary copy{d};
    Vocabulary v{copy};

    WHEN("asked for its size") {
      THEN("the vocabulary is the size of the copied vocabulary") {
        REQUIRE(copy.size() == 2);
        REQUIRE(copy.size() == copy.length());

        REQUIRE(v.size() == copy.size());
        REQUIRE(v.length() == copy.length());
      }
    }

    WHEN("asked if it contains document items")  {
      THEN("it does have them") {
        for (auto const& t : d)
          REQUIRE(v.is_in(t));
      }
    }
  }

  GIVEN("a copy of a vocabulary using move semantics") {
    Vocabulary copy{d};
    Vocabulary v{std::move(copy)};

    WHEN("asked for its size") {
      THEN("the vocabulary is the size of the moved vocabulary") {
        REQUIRE(v.size() == 2);
        REQUIRE(v.length() == v.size());
      }
    }

    WHEN("asked if it contains document items")  {
      THEN("it does have them") {
        for (auto const& t : d)
          REQUIRE(v.is_in(t));
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

    REQUIRE(v1.size() == 2);
    REQUIRE(v1.length() == 2);
    REQUIRE(v1.size() == v1.length());
    REQUIRE(v1.is_in("t2"));

    REQUIRE(v2.size() == 2);
    REQUIRE(v2.length() == 2);
    REQUIRE(v2.size() == v2.length());

    WHEN("it is subtracted by another vocabulary") {
      v1 = v1 - v2;

      THEN("the shared tokens are removed") {
        REQUIRE(v1.size() == 1);
        REQUIRE(v1.length() == 1);
        REQUIRE(v1.size() == v1.length());
        REQUIRE(!v1.is_in("t2"));
      }
    }

    WHEN("it is subtracted by another vocabulary") {
      v1 -= v2;

      THEN("the shared tokens are removed") {
        REQUIRE(v1.size() == 1);
        REQUIRE(v1.length() == 1);
        REQUIRE(v1.size() == v1.length());
        REQUIRE(!v1.is_in("t2"));
      }
    }

    WHEN("it is added by another vocabulary") {
      v1 = v1 + v2;

      THEN("the unshared tokens are added") {
        REQUIRE(v1.size() == 3);
        REQUIRE(v1.length() == v1.size());
        REQUIRE(v1.is_in("t3"));
      }
    }

    WHEN("it is added by another vocabulary") {
      v1 += v2;

      THEN("the unshared tokens are added") {
        REQUIRE(v1.size() == 3);
        REQUIRE(v1.length() == v1.size());
        REQUIRE(v1.is_in("t3"));
      }
    }
  }
}

SCENARIO("a vocabulary can be queried if it constains certain token", "[query]") {
  Document d{"t1", "t2", "t1", "t3"};
  Vocabulary v{d};

  REQUIRE(v.size() == 3);

  GIVEN("a non-empty vocabulary") {
    WHEN("asked if it contains an existing item") {
      THEN("it should answer it has") {
        REQUIRE(v.is_in("t1"));
        REQUIRE(v.is_in("t2"));
        REQUIRE(v.is_in("t3"));
      }

      WHEN("asked if it contains an non-existing item") {
        THEN("it should answet it has not") {
          REQUIRE(!v.is_in("t0"));
          REQUIRE(!v.is_in("t4"));
        }
      }
    }
  }
}
