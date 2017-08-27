#ifndef TEXMER_TOPIC_INDICATOR_SEQUENCE_H_
#define TEXMER_TOPIC_INDICATOR_SEQUENCE_H_

#include "sequence.h"
#include "def.h"

namespace texmer {

  using TopicIndicatorSequence = Sequence<size_t>;
  using DocumentTopicIndicatorSequences = DocumentSequences<TopicIndicatorSequence>;
  using CorpusTopicIndicatorSequences = Vector<DocumentTopicIndicatorSequences>;

} // namespace texmer

#endif // TEXMER_TOPIC_INDICATORSEQUENCE_H_
