#ifndef TOPIC_INDICATOR_TRANSFORMER_H
#define TOPIC_INDICATOR_TRANSFORMER_H

class TopicIndicatorTransformer {
public:
  TopicIndicatorTransformer(std::size_t sentence_size);

  ~TopicIndicatorTransformer() = default;

  CorpusTopicIndicatorSequences transform(const Corpus& tokens, const TypeTopicIndicatorMode& modes) const;
  DocumenTopicIndicatorSequences transform(const Document& tokens, const TypeTopicIndicatorMode& modes) const;

private:
  std::size_t sentence_size_;

  TopicIndicatorSequence tokens_to_topic_indicators(const Document& tokens, const TypeTopicMode& modes) const;

};


#endif // TOPIC_INDICATOR_TRANSFORMER_H
