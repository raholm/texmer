#ifndef TYPE_TOPIC_INDICATOR_MODE_H
#define TYPE_TOPIC_INDICATOR_MODE_H

#include <vector>
#include <map>

class TypeTopicIndicatorMode {
public:
  using Type = std::string;
  using TopicIndicator = std::size_t;
  using Count = std::size_t;
  using TTPair = std::pair<Type, TopicIndicator>;
  using TTCPair = std::pair<TTPair, Count>;

  TypeTopicIndicatorMode(const std::vector<Type>& types,
                         const std::vector<TopicIndicator>& topic_indicators);
  TypeTopicIndicatorMode(const std::vector<TTPair>& type_topic_indicators);

  TypeTopicIndicatorMode(const TypeTopicIndicatorMode& other) = default;
  TypeTopicIndicatorMode(TypeTopicIndicatorMode&& other) = default;

  ~TypeTopicIndicatorMode() = default;

  std::size_t size() const;

  bool contains(const Type& type) const;

  TopicIndicator get_mode(const Type& type) const;

private:
  std::map<Type, TopicIndicator> type_topic_mode_;
  std::map<TTPair, Count> type_topic_count_;

  void insert_or_add_element(const TTCPair& pair);

};

#endif
