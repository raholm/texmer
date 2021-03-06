#ifndef TEXMER_TYPE_TOPIC_INDICATOR_MODE_H_
#define TEXMER_TYPE_TOPIC_INDICATOR_MODE_H_

#include "def.h"

namespace texmer {

  class TypeTopicIndicatorMode {
  public:
    using type = String;
    using topic_indicator = size_t;
    using type_topic_indicator_pair = Pair<type, topic_indicator>;

    using type_vector = Vector<type>;
    using topic_indicator_vector = Vector<topic_indicator>;
    using type_topic_indicator_pair_vector = Vector<type_topic_indicator_pair>;

  private:
    using count = size_t;
    using type_topic_indicator_count_pair = Pair<type_topic_indicator_pair, count>;

  public:
    TypeTopicIndicatorMode() = default;
    TypeTopicIndicatorMode(const type_vector& types,
                           const topic_indicator_vector& topic_indicators);
    TypeTopicIndicatorMode(const type_topic_indicator_pair_vector& type_topic_indicators);

    TypeTopicIndicatorMode(const TypeTopicIndicatorMode& other) = default;
    TypeTopicIndicatorMode(TypeTopicIndicatorMode&& other) = default;

    ~TypeTopicIndicatorMode() = default;

    void update(const type_vector& types,
                const topic_indicator_vector& topic_indicators);
    void update(const type_topic_indicator_pair_vector& type_topic_indicators);

    inline size_t size() const {
      return type_topic_mode_.size();
    }

    inline bool contains(const type& type) const {
      return type_topic_mode_.find(type) != type_topic_mode_.end();
    }

    inline Vector<type_topic_indicator_pair> get_data() const {
      return Vector<type_topic_indicator_pair>{type_topic_mode_.cbegin(), type_topic_mode_.cend()};
    }

    inline topic_indicator get_mode(const type& type) const {
      auto it = type_topic_mode_.find(type);
      if (it == type_topic_mode_.end())
        throw std::out_of_range("Type has no mode.");
      return it->second;
    }

  private:
    Map<type, topic_indicator> type_topic_mode_;
    Map<type_topic_indicator_pair, count> type_topic_count_;

    void insert_or_add_element(const type_topic_indicator_count_pair& pair);

  };

} // namespace texmer

#endif // TEXMER_TYPE_TOPIC_INDICATOR_MODE_H_
