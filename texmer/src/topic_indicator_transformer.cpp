#include "topic_indicator_transformer.h"

#include <cmath>

TopicIndicatorTransformer::TopicIndicatorTransformer(std::size_t sentence_size)
  : sentence_size_(sentence_size)
{
  if (sentence_size_ < 1) {
    throw std::invalid_argument("Invalid sentence size: '" + std::to_string(sentence_size_) + "'.");
  }
}

CorpusTopicIndicatorSequences TopicIndicatorTransformer::transform(const Corpus& tokens,
                                                                   const StringVector& stopwords,
                                                                   const TypeTopicIndicatorMode& modes) const {
  return transform(tokens, Vocabulary(stopwords), modes);
}

CorpusTopicIndicatorSequences TopicIndicatorTransformer::transform(const Corpus& tokens,
                                                                   const Vocabulary& stopwords,
                                                                   const TypeTopicIndicatorMode& modes) const {
  CorpusTopicIndicatorSequences topic_indicator_sequences(tokens.size());

  for (unsigned i = 0; i < tokens.size(); ++i) {
    topic_indicator_sequences.at(i) = transform(tokens.at(i), stopwords, modes);
  }

  return topic_indicator_sequences;
}

DocumentTopicIndicatorSequences TopicIndicatorTransformer::transform(const Document& tokens,
                                                                     const StringVector& stopwords,
                                                                     const TypeTopicIndicatorMode& modes) const {
  return transform(tokens, Vocabulary(stopwords), modes);
}

DocumentTopicIndicatorSequences TopicIndicatorTransformer::transform(const Document& tokens,
                                                                     const Vocabulary& stopwords,
                                                                     const TypeTopicIndicatorMode& modes) const {
  std::size_t n_segs = ceil(tokens.size() / sentence_size_);
  DocumentTopicIndicatorSequences topic_indicator_sequences;
  topic_indicator_sequences.reserve(n_segs);

  std::size_t start, end;
  Document segment;
  TopicIndicatorSequence topic_indicator_sequence;

  for (unsigned i = 0; i < n_segs; ++i) {
    start = i * sentence_size_;
    end = std::min(start + sentence_size_, tokens.size());

    segment = Document(tokens.begin() + start, tokens.begin() + end);

    topic_indicator_sequence = tokens_to_topic_indicators(segment, stopwords, modes);

    // TODO: Is this what we want?
    if (topic_indicator_sequence.length() == 0)
      continue;

    topic_indicator_sequences.push_back(topic_indicator_sequence);
  }

  return topic_indicator_sequences;
}

TopicIndicatorSequence
TopicIndicatorTransformer::tokens_to_topic_indicators(const Document& tokens,
                                                      const Vocabulary& stopwords,
                                                      const TypeTopicIndicatorMode& modes) const {
  std::vector<TopicIndicatorSequence::key> topics;
  topics.reserve(tokens.size());

  // TODO: Should we check that token has a mode and is not a stopword?
  for (const auto& token : tokens)
    if (!stopwords.contains(token) && modes.contains(token))
      topics.push_back(modes.get_mode(token));

  return TopicIndicatorSequence(topics);
}
