#ifndef TOPIC_INDICATOR_SEQUENCE_H
#define TOPIC_INDICATOR_SEQUENCE_H

#include <vector>
#include <map>

class TopicIndicatorSequence {
public:
  using Topic = std::size_t;
  using Count = std::size_t;

  TopicIndicatorSequence() = default;
  TopicIndicatorSequence(const std::vector<Topic>& topics);

  TopicIndicatorSequence(const TopicIndicatorSequence& other) = default;
  TopicIndicatorSequence(TopicIndicatorSequence&& other) = default;

  ~TopicIndicatorSequence() = default;

  TopicIndicatorSequence operator+(const TopicIndicatorSequence& rhs) const;
  TopicIndicatorSequence& operator+=(const TopicIndicatorSequence& rhs);

  TopicIndicatorSequence operator*(const TopicIndicatorSequence& rhs) const;
  TopicIndicatorSequence& operator*=(const TopicIndicatorSequence& rhs);

  bool operator==(const TopicIndicatorSequence& rhs) const;
  bool operator!=(const TopicIndicatorSequence& rhs) const;

  std::size_t size() const;
  std::size_t length() const;

  std::vector<Topic> get_topics() const;
  std::vector<Count> get_counts() const;

private:
  std::map<Topic, Count> topic_count_;

  void insert_or_add_element(const std::pair<Topic, std::size_t>& element);

};

#endif // TOPIC_INDICATORSEQUENCE_H
