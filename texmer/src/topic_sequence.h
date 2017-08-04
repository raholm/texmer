#ifndef TOPIC_SEQUENCE_H
#define TOPIC_SEQUENCE_H

#include <vector>
#include <map>

class TopicSequence {
public:
  using Topic = std::size_t;

  TopicSequence() = default;
  TopicSequence(const std::vector<Topic>& topics);

  TopicSequence(const TopicSequence& other) = default;
  TopicSequence(TopicSequence&& other) = default;

  ~TopicSequence() = default;

  TopicSequence operator+(const TopicSequence& rhs) const;
  TopicSequence& operator+=(const TopicSequence& rhs);

  TopicSequence operator*(const TopicSequence& rhs) const;
  TopicSequence& operator*=(const TopicSequence& rhs);

  bool operator==(const TopicSequence& rhs) const;
  bool operator!=(const TopicSequence& rhs) const;

  std::size_t size() const;
  std::size_t length() const;

  std::vector<Topic> get_topics() const;
  std::vector<std::size_t> get_counts() const;

private:
  std::map<Topic, std::size_t> topic_count_;

  void insert_or_add_element(const std::pair<Topic, std::size_t>& element);

};

#endif // TOPIC_SEQUENCE_H
