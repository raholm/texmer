#include <catch.hpp>

#include "def.h"
#include "topic_indicator_transformer.h"
#include "type_topic_indicator_mode.h"
#include "topic_indicator_sequence.h"
#include "test_helper.h"

namespace texmer {
  namespace test {

    SCENARIO("a topic indicator transformer requires a positive sentence size", "[constructor]") {
      GIVEN("a sentence size of 0") {
        THEN("it should throw an invalid argument error") {
          REQUIRE_THROWS_AS(TopicIndicatorTransformer(0), std::invalid_argument);
        }
      }

      GIVEN("a positive sentence size") {
        THEN("it should not throw an invalid argument error") {
          REQUIRE_NOTHROW(TopicIndicatorTransformer(1));
        }
      }
    }

    SCENARIO("a topic indicator transformer should transform tokens to topic indicator sequences", "[transform]") {
      Document doc{"is", "good", "what", "are", "that", "doing", "here", "here"};
      IntVector topic_indicators{1, 1, 1, 2, 1, 1, 2, 2};
      TypeTopicIndicatorMode modes{doc, topic_indicators};
      size_t sentence_size = 2;
      StringVector stopwords{"is", "that"};

      Corpus corpus;
      corpus.push_back(doc);
      corpus.push_back(doc);

      GIVEN("a document and sentence size of 2") {
        TopicIndicatorTransformer trans{sentence_size};
        THEN("it should return topic indicator sequences") {
          auto sequences = trans.transform(doc, stopwords, modes);
          check_equality(sequences, DocumentTopicIndicatorSequences{
              TopicIndicatorSequence({1}),
                TopicIndicatorSequence({1, 2}),
                TopicIndicatorSequence({1}),
                TopicIndicatorSequence({2, 2})});
        }
      }

      GIVEN("a corpus and sentence size of 2") {
        TopicIndicatorTransformer trans{sentence_size};
        THEN("it should return topic indicator sequences") {
          auto sequences = trans.transform(corpus, stopwords, modes);
          check_equality(sequences, CorpusTopicIndicatorSequences{{
                TopicIndicatorSequence({1}),
                  TopicIndicatorSequence({1, 2}),
                  TopicIndicatorSequence({1}),
                  TopicIndicatorSequence({2, 2})},{
                TopicIndicatorSequence({1}),
                  TopicIndicatorSequence({1, 2}),
                  TopicIndicatorSequence({1}),
                  TopicIndicatorSequence({2, 2})
                  }});
        }
      }
    }

  } // namespace test
} // namespace texmer
