#include <testthat.h>

#include "def.h"
#include "topic_indicator_transformer.h"
#include "type_topic_indicator_mode.h"
#include "topic_indicator_sequence.h"
#include "test_helper.h"

namespace texmer {
  namespace test {

    context("topic indicator transformer"){
      test_that("topic indicator transformer constructs correctly") {
        expect_error_as(TopicIndicatorTransformer(0), std::invalid_argument);
        expect_no_error(TopicIndicatorTransformer(1));
      }

      test_that("topic indicator transformer transforms document correctly") {
        Document doc{"is", "good", "what", "are", "that", "doing", "here", "here"};
        IntVector topic_indicators{1, 1, 1, 2, 1, 1, 2, 2};
        TypeTopicIndicatorMode modes{doc, topic_indicators};
        size_t sentence_size = 2;
        StringVector stopwords{"is", "that"};

        TopicIndicatorTransformer trans{sentence_size};
        auto sequences = trans.transform(doc, stopwords, modes);
        check_equality(sequences, DocumentTopicIndicatorSequences{
            TopicIndicatorSequence({1}),
              TopicIndicatorSequence({1, 2}),
              TopicIndicatorSequence({1}),
              TopicIndicatorSequence({2, 2})});
      }

      test_that("topic indicator transformer transforms corpus correctly") {
        Document doc{"is", "good", "what", "are", "that", "doing", "here", "here"};
        Corpus corpus{doc, doc};
        IntVector topic_indicators{1, 1, 1, 2, 1, 1, 2, 2};
        TypeTopicIndicatorMode modes{doc, topic_indicators};
        size_t sentence_size = 2;
        StringVector stopwords{"is", "that"};

        TopicIndicatorTransformer trans{sentence_size};
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

  } // namespace test
} // namespace texmer
