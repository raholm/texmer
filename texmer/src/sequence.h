#ifndef TEXMER_SEQUENCE_H_
#define TEXMER_SEQUENCE_H_

#include <algorithm>
#include <numeric>

#include "def.h"

namespace texmer {

  class SequenceBase {
  public:
    virtual ~SequenceBase() = default;

  };

  template<typename Key>
  class Sequence : public SequenceBase {
  public:
    using KeyType = Key;
    using ValueType = size_t;
    using KeyValuePair = Pair<KeyType, ValueType>;

    Sequence() = default;
    Sequence(const Sequence& other) = default;
    Sequence(Sequence&& other) = default;
    Sequence(const Vector<KeyType>& keys) {
      for (auto const& key : keys)
        insert_or_add_element(std::make_pair(key, 1));
    }

    ~Sequence() = default;

    Sequence& operator=(const Sequence& other) = default;
    Sequence& operator=(Sequence&& other) = default;

    inline Sequence operator+(const Sequence& rhs) const {
      Sequence tmp(*this);
      return tmp += rhs;
    }

    inline Sequence& operator+=(const Sequence& rhs) {
      for (auto const &p : rhs.map_)
        insert_or_add_element(p);
      return *this;
    }

    inline Sequence operator*(const Sequence& rhs) const {
      Sequence tmp(*this);
      return tmp *= rhs;
    }

    inline Sequence& operator*=(const Sequence& rhs) {
      for (auto it = map_.cbegin(); it != map_.cend();) {
        auto rit = rhs.map_.find(it->first);
        if (rit != rhs.map_.end()) {
          map_.at(it->first) = it->second * rit->second;
          it++;
        } else it = map_.erase(it);
      }
      return *this;
    }

    inline bool operator==(const Sequence& rhs) const {
      return size() == rhs.size() &&
        std::equal(map_.cbegin(), map_.cend(), rhs.map_.cbegin());
    }

    inline bool operator!=(const Sequence& rhs) const {
      return !(*this == rhs);
    }

    inline size_t size() const {
      return map_.size();
    }

    inline size_t length() const {
      return std::accumulate(map_.cbegin(), map_.cend(), 0,
                             [](const size_t acc,
                                KeyValuePair const& p) {
                               return acc + p.second;
                             });
    }

    inline Vector<KeyType> get_keys() const {
      Vector<KeyType> keys;
      keys.reserve(size());
      std::transform(map_.cbegin(), map_.cend(), std::back_inserter(keys),
                     [](KeyValuePair const& pair) {
                       return pair.first;
                     });
      return keys;
    }

    inline Vector<ValueType> get_values() const {
      Vector<ValueType> values;
      values.reserve(size());
      std::transform(map_.cbegin(), map_.cend(), std::back_inserter(values),
                     [](KeyValuePair const& pair) {
                       return pair.second;
                     });
      return values;
    }

  protected:
    Map<KeyType, ValueType> map_;

  private:
    inline void insert_or_add_element(const KeyValuePair& element) {
      if (!map_.insert(element).second)
        map_.at(element.first) += element.second;
    }

  };

  template<typename T>
  using DocumentSequences = Vector<T>;

  template<typename T>
  using CorpusSequences = Vector<DocumentSequences<T>>;

  template<typename T>
  using BlockSequences = DocumentSequences<T>;

} // namespace texmer

#endif // TEXMER_SEQUENCE_H_
