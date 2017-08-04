#include "type_topic_indicator_mode.h"

#include <exception>

using Type = TypeTopicIndicatorMode::Type;
using TopicIndicator = TypeTopicIndicatorMode::TopicIndicator;
using TTPair = TypeTopicIndicatorMode::TTPair;
using TTCPair = TypeTopicIndicatorMode::TTCPair;

TypeTopicIndicatorMode::TypeTopicIndicatorMode(const std::vector<Type>& types,
                                               const std::vector<TopicIndicator>& topic_indicators) {
  if (types.size() != topic_indicators.size())
    throw std::invalid_argument("Types and Topic Indicators have different sizes.");

  for (unsigned i = 0; i < types.size(); ++i)
    insert_or_add_element(std::make_pair(std::make_pair(types.at(i), topic_indicators.at(i)), 1));
}

TypeTopicIndicatorMode::TypeTopicIndicatorMode(const std::vector<TTPair>& type_topic_indicators) {
  for (auto const& pair : type_topic_indicators)
    insert_or_add_element(std::make_pair(pair, 1));
}

std::size_t TypeTopicIndicatorMode::size() const {
  return type_topic_mode_.size();
}

TopicIndicator TypeTopicIndicatorMode::get_mode(const Type& type) const {
  return type_topic_mode_.at(type);
}

void TypeTopicIndicatorMode::insert_or_add_element(const TTCPair& element) {
  if (!type_topic_count_.insert(element).second) {
    type_topic_count_.at(element.first) += element.second;
  }

  Type type = element.first.first;
  TopicIndicator topic_indicator = element.first.second;

  if (type_topic_count_.at(element.first) > type_topic_count_.at(std::make_pair(type, get_mode(type))))
    type_topic_mode_.at(type) = topic_indicator;
}
