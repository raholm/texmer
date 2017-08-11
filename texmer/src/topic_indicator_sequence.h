#ifndef TEXMER_TOPIC_INDICATOR_SEQUENCE_H_
#define TEXMER_TOPIC_INDICATOR_SEQUENCE_H_

#include "sequence.h"
#include "def.h"

namespace texmer {

  // class TopicIndicatorSequence : public Sequence<size_t> {
  // public:
  //   using base_class = Sequence<size_t>;
  //   using key = base_class::key;
  //   using value = base_class::value;

  //   TopicIndicatorSequence() : base_class() {}
  //   TopicIndicatorSequence(const TopicIndicatorSequence& other) : base_class(other) {}
  //   TopicIndicatorSequence(TopicIndicatorSequence&& other) : base_class(std::move(other)) {}
  //   TopicIndicatorSequence(const Vector<key>& topic_indicators) : base_class(topic_indicators) {}

  //   ~TopicIndicatorSequence() = default;

  //   inline TopicIndicatorSequence& operator=(const TopicIndicatorSequence& rhs) {
  //     base_class::operator=(rhs);
  //     return *this;
  //   }

  //   inline TopicIndicatorSequence& operator=(TopicIndicatorSequence&& rhs) {
  //     base_class::operator=(std::move(rhs));
  //     return *this;
  //   }

  //   inline TopicIndicatorSequence operator+(const TopicIndicatorSequence& rhs) const {
  //     TopicIndicatorSequence copy(*this);
  //     return copy += rhs;
  //   }

  //   inline TopicIndicatorSequence& operator+=(const TopicIndicatorSequence& rhs) {
  //     base_class::operator+=(rhs);
  //     return *this;
  //   }

  //   inline TopicIndicatorSequence operator*(const TopicIndicatorSequence& rhs) const {
  //     TopicIndicatorSequence copy(*this);
  //     return copy *= rhs;
  //   }

  //   inline TopicIndicatorSequence& operator*=(const TopicIndicatorSequence& rhs) {
  //     base_class::operator*=(rhs);
  //     return *this;
  //   }

  //   inline bool operator==(const TopicIndicatorSequence& rhs) const {
  //     return base_class::operator==(rhs);
  //   }

  //   inline bool operator!=(const TopicIndicatorSequence& rhs) const {
  //     return base_class::operator!=(rhs);
  //   }

  // };

  using TopicIndicatorSequence = Sequence<size_t>;
  using DocumentTopicIndicatorSequences = DocumentSequences<TopicIndicatorSequence>;
  using CorpusTopicIndicatorSequences = Vector<DocumentTopicIndicatorSequences>;

} // namespace texmer

#endif // TEXMER_TOPIC_INDICATORSEQUENCE_H_
