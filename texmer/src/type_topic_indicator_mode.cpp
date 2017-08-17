#include "type_topic_indicator_mode.h"

#include <exception>

namespace texmer {

  TypeTopicIndicatorMode::TypeTopicIndicatorMode(const TypeTopicIndicatorMode::type_vector& types,
                                                 const TypeTopicIndicatorMode::topic_indicator_vector& topic_indicators) {
    if (types.size() != topic_indicators.size())
      throw std::invalid_argument("Types and Topic Indicators have different sizes.");

    for (unsigned i = 0; i < types.size(); ++i)
      insert_or_add_element(std::make_pair(std::make_pair(types.at(i), topic_indicators.at(i)), 1));
  }

  TypeTopicIndicatorMode::TypeTopicIndicatorMode(const TypeTopicIndicatorMode::type_topic_indicator_pair_vector& type_topic_indicators) {
    for (auto const& pair : type_topic_indicators)
      insert_or_add_element(std::make_pair(pair, 1));
  }

  void TypeTopicIndicatorMode::update(const type_vector& types,
                                      const topic_indicator_vector& topic_indicators) {
    if (types.size() != topic_indicators.size())
      throw std::invalid_argument("Types and Topic Indicators have different sizes.");

    for (unsigned i = 0; i < types.size(); ++i)
      insert_or_add_element(std::make_pair(std::make_pair(types.at(i), topic_indicators.at(i)), 1));
  }

  void TypeTopicIndicatorMode::update(const type_topic_indicator_pair_vector& type_topic_indicators) {
    for (auto const& pair : type_topic_indicators)
      insert_or_add_element(std::make_pair(pair, 1));
  }

  void TypeTopicIndicatorMode::insert_or_add_element(const TypeTopicIndicatorMode::type_topic_indicator_count_pair& element) {
    if (!type_topic_count_.insert(element).second) {
      type_topic_count_[element.first] += element.second;
    }

    auto type = element.first.first;
    auto topic_indicator = element.first.second;

    if (contains(type)) {
      if (type_topic_count_[element.first] > type_topic_count_[std::make_pair(type, get_mode(type))])
        type_topic_mode_[type] = topic_indicator;
    } else {
      type_topic_mode_[type] = topic_indicator;
    }
  }

}
