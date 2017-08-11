#ifndef TEXMER_TOPIC_INDICATOR_TRANSFORMER_H_
#define TEXMER_TOPIC_INDICATOR_TRANSFORMER_H_

#include <exception>
#include <string>

#include "def.h"
#include "topic_indicator_sequence.h"
#include "type_topic_indicator_mode.h"
#include "vocabulary.h"

namespace texmer {

  class TopicIndicatorTransformer {
  public:
    TopicIndicatorTransformer(size_t sentence_size)
      : sentence_size_(sentence_size) {
      if (sentence_size_ < 1) {
        throw std::invalid_argument("Invalid sentence size: '" + std::to_string(sentence_size_) + "'.");
      }
    }


    ~TopicIndicatorTransformer() = default;

    CorpusTopicIndicatorSequences transform(const Corpus& tokens,
                                            const StringVector& stopwords,
                                            const TypeTopicIndicatorMode& modes) const;
    CorpusTopicIndicatorSequences transform(const Corpus& tokens,
                                            const Vocabulary& stopwords,
                                            const TypeTopicIndicatorMode& modes) const;

    DocumentTopicIndicatorSequences transform(const Document& tokens,
                                              const StringVector& stopwords,
                                              const TypeTopicIndicatorMode& modes) const;
    DocumentTopicIndicatorSequences transform(const Document& tokens,
                                              const Vocabulary& stopwords,
                                              const TypeTopicIndicatorMode& modes) const;

  private:
    size_t sentence_size_;

    TopicIndicatorSequence tokens_to_topic_indicators(const Document& tokens,
                                                      const Vocabulary& stopwords,
                                                      const TypeTopicIndicatorMode& modes) const;

  };

} // namespace texmer

#endif // TEXMER_TOPIC_INDICATOR_TRANSFORMER_H_
