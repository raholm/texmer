#ifndef TOPIC_INDICATOR_TRANSFORMER_H
#define TOPIC_INDICATOR_TRANSFORMER_H

#include "def.h"
#include "topic_indicator_sequence.h"
#include "type_topic_indicator_mode.h"
#include "vocabulary.h"

class TopicIndicatorTransformer {
public:
  TopicIndicatorTransformer(std::size_t sentence_size);

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
  std::size_t sentence_size_;

  TopicIndicatorSequence tokens_to_topic_indicators(const Document& tokens,
                                                    const Vocabulary& stopwords,
                                                    const TypeTopicIndicatorMode& modes) const;

};

#endif // TOPIC_INDICATOR_TRANSFORMER_H
