#include "topic_indicator_sequence.h"

#include <algorithm>
#include <numeric>

using Topic = TopicIndicatorSequence::Topic;

TopicIndicatorSequence::TopicIndicatorSequence(const std::vector<Topic>& topics) {
  for (auto const& topic : topics)
    insert_or_add_element(std::make_pair(topic, 1));
}

TopicIndicatorSequence TopicIndicatorSequence::operator+(const TopicIndicatorSequence& rhs) const {
  TopicIndicatorSequence tmp(*this);
  return tmp += rhs;
}

TopicIndicatorSequence& TopicIndicatorSequence::operator+=(const TopicIndicatorSequence& rhs) {
  for (auto const &p : rhs.topic_count_) {
    insert_or_add_element(p);
  }
  return *this;
}

TopicIndicatorSequence TopicIndicatorSequence::operator*(const TopicIndicatorSequence& rhs) const {
  TopicIndicatorSequence tmp(*this);
  return tmp *= rhs;
}

TopicIndicatorSequence& TopicIndicatorSequence::operator*=(const TopicIndicatorSequence& rhs) {
  for (auto it = topic_count_.cbegin(); it != topic_count_.cend();) {
    auto rit = rhs.topic_count_.find(it->first);
    if (rit != rhs.topic_count_.end()) {
      topic_count_.at(it->first) = it->second * rit->second;
      it++;
    } else it = topic_count_.erase(it);
  }
  return *this;
}

bool TopicIndicatorSequence::operator==(const TopicIndicatorSequence& rhs) const {
  return topic_count_.size() == rhs.topic_count_.size() &&
    std::equal(topic_count_.cbegin(), topic_count_.cend(), rhs.topic_count_.cbegin());
}

bool TopicIndicatorSequence::operator!=(const TopicIndicatorSequence& rhs) const {
  return !(*this == rhs);
}

std::size_t TopicIndicatorSequence::size() const {
  return topic_count_.size();
}

std::size_t TopicIndicatorSequence::length() const {
  return std::accumulate(topic_count_.cbegin(), topic_count_.cend(), 0,
                         [](const std::size_t acc,
                            auto const& p) {
                           return acc + p.second;
                         });
}

std::vector<Topic> TopicIndicatorSequence::get_topics() const {
  std::vector<Topic> topics;
  topics.reserve(size());
  std::transform(topic_count_.cbegin(), topic_count_.cend(), std::back_inserter(topics),
                 [](auto const& pair) {
                   return pair.first;
                 });
  return topics;
}

std::vector<std::size_t> TopicIndicatorSequence::get_counts() const {
  std::vector<std::size_t> counts;
  counts.reserve(size());
  std::transform(topic_count_.cbegin(), topic_count_.cend(), std::back_inserter(counts),
                 [](auto const& pair) {
                   return pair.second;
                 });
  return counts;
}

void TopicIndicatorSequence::insert_or_add_element(const std::pair<Topic, std::size_t>& element) {
  if (!topic_count_.insert(element).second) {
    topic_count_.at(element.first) += element.second;
  }
}
