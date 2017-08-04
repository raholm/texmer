#include "topic_sequence.h"

#include <algorithm>
#include <numeric>

TopicSequence::TopicSequence(const std::vector<TopicSequence::Topic>& topics) {
  for (auto const& topic : topics)
    insert_or_add_element(std::make_pair(topic, 1));
}

TopicSequence TopicSequence::operator+(const TopicSequence& rhs) const {
  TopicSequence tmp(*this);
  return tmp += rhs;
}

TopicSequence& TopicSequence::operator+=(const TopicSequence& rhs) {
  for (auto const &p : rhs.topic_count_) {
    insert_or_add_element(p);
  }
  return *this;
}

TopicSequence TopicSequence::operator*(const TopicSequence& rhs) const {
  TopicSequence tmp(*this);
  return tmp *= rhs;
}

TopicSequence& TopicSequence::operator*=(const TopicSequence& rhs) {
  for (auto it = topic_count_.cbegin(); it != topic_count_.cend();) {
    auto rit = rhs.topic_count_.find(it->first);
    if (rit != rhs.topic_count_.end()) {
      topic_count_.at(it->first) = it->second * rit->second;
      it++;
    } else it = topic_count_.erase(it);
  }
  return *this;
}


bool TopicSequence::operator==(const TopicSequence& rhs) const {
  return topic_count_.size() == rhs.topic_count_.size() &&
    std::equal(topic_count_.cbegin(), topic_count_.cend(),
               rhs.topic_count_.cbegin());
}

bool TopicSequence::operator!=(const TopicSequence& rhs) const {
  return !(*this == rhs);
}

std::size_t TopicSequence::size() const {
  return topic_count_.size();
}

std::size_t TopicSequence::length() const {
  return std::accumulate(topic_count_.cbegin(), topic_count_.cend(), 0,
                         [](const std::size_t acc,
                            auto const& p) {
                           return acc + p.second;
                         });
}

std::vector<TopicSequence::Topic> TopicSequence::get_topics() const {
  std::vector<TopicSequence::Topic> topics;
  topics.reserve(size());
  std::transform(topic_count_.cbegin(), topic_count_.cend(), std::back_inserter(topics),
                 [](auto const& pair) {
                   return pair.first;
                 });
  return topics;
}

std::vector<std::size_t> TopicSequence::get_counts() const {
  std::vector<std::size_t> counts;
  counts.reserve(size());
  std::transform(topic_count_.cbegin(), topic_count_.cend(), std::back_inserter(counts),
                 [](auto const& pair) {
                   return pair.second;
                 });
  return counts;
}


void TopicSequence::insert_or_add_element(const std::pair<TopicSequence::Topic, std::size_t>& element) {
  if (!topic_count_.insert(element).second) {
    topic_count_[element.first] += element.second;
  }
}
