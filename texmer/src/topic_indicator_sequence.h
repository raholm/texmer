#ifndef TOPIC_INDICATOR_SEQUENCE_H
#define TOPIC_INDICATOR_SEQUENCE_H

#include <vector>
#include <map>

#include "sequence.h"

class TopicIndicatorSequence : public Sequence<std::size_t> {
public:
  using base = Sequence<std::size_t>;
  using topic_indicator = typename base::key;
  using count = typename base::value;

  TopicIndicatorSequence() : base() {}
  TopicIndicatorSequence(const TopicIndicatorSequence& other) : base(other) {}
  TopicIndicatorSequence(TopicIndicatorSequence&& other) : base(std::move(other)) {}
  TopicIndicatorSequence(const std::vector<topic_indicator>& topic_indicators) : base(topic_indicators) {}

  ~TopicIndicatorSequence() = default;

  inline TopicIndicatorSequence& operator=(const TopicIndicatorSequence& rhs) {
    base::operator=(rhs);
    return *this;
  }

  inline TopicIndicatorSequence& operator=(TopicIndicatorSequence&& rhs) {
    base::operator=(std::move(rhs));
    return *this;
  }

  inline TopicIndicatorSequence operator+(const TopicIndicatorSequence& rhs) const {
    TopicIndicatorSequence copy(*this);
    return copy += rhs;
  }

  inline TopicIndicatorSequence& operator+=(const TopicIndicatorSequence& rhs) {
    base::operator+=(rhs);
    return *this;
  }

  inline TopicIndicatorSequence operator*(const TopicIndicatorSequence& rhs) const {
    TopicIndicatorSequence copy(*this);
    return copy *= rhs;
  }

  inline TopicIndicatorSequence& operator*=(const TopicIndicatorSequence& rhs) {
    base::operator*=(rhs);
    return *this;
  }

  inline bool operator==(const TopicIndicatorSequence& rhs) const {
    return base::operator==(rhs);
  }

  inline bool operator!=(const TopicIndicatorSequence& rhs) const {
    return base::operator!=(rhs);
  }

};

using DocumentTopicIndicatorSequences = DocumentSequences<TopicIndicatorSequence>;
using CorpusTopicIndicatorSequences = std::vector<DocumentTopicIndicatorSequences>;

#endif // TOPIC_INDICATORSEQUENCE_H
