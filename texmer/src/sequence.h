#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <vector>
#include <map>
#include <algorithm>
#include <numeric>

template<typename K>
class Sequence {
public:
  using key = K;
  using value = std::size_t;

  Sequence() = default;
  Sequence(const Sequence<key>& other) = default;
  Sequence(Sequence<key>&& other) = default;

  Sequence(const std::vector<key>& keys) {
    for (auto const& key : keys)
      insert_or_add_element(std::make_pair(key, 1));
  }

  virtual ~Sequence() = default;

  Sequence<key>& operator=(const Sequence<key>& other) = default;
  Sequence<key>& operator=(Sequence<key>&& other) = default;

  Sequence<key> operator+(const Sequence<key>& rhs) const {
    Sequence<K> tmp(*this);
    return tmp += rhs;
  }

  Sequence<key>& operator+=(const Sequence<key>& rhs) {
    for (auto const &p : rhs.map_) {
      insert_or_add_element(p);
    }
    return *this;
  }

  Sequence<key> operator*(const Sequence<key>& rhs) const {
    Sequence<K> tmp(*this);
    return tmp *= rhs;
  }

  Sequence<key>& operator*=(const Sequence<key>& rhs) {
    for (auto it = map_.cbegin(); it != map_.cend();) {
      auto rit = rhs.map_.find(it->first);
      if (rit != rhs.map_.end()) {
        map_.at(it->first) = it->second * rit->second;
        it++;
      } else it = map_.erase(it);
    }
    return *this;
  }

  bool operator==(const Sequence<key>& rhs) const {
    return size() == rhs.size() &&
      std::equal(map_.cbegin(), map_.cend(), rhs.map_.cbegin());
  }

  bool operator!=(const Sequence<key>& rhs) const {
    return !(*this == rhs);
  }

  std::size_t size() const {
    return map_.size();
  }

  std::size_t length() const {
    return std::accumulate(map_.cbegin(), map_.cend(), 0,
                           [](const std::size_t acc,
                              auto const& p) {
                             return acc + p.second;
                           });
  }

  std::vector<key> get_keys() const {
    std::vector<K> keys;
    keys.reserve(size());
    std::transform(map_.cbegin(), map_.cend(), std::back_inserter(keys),
                   [](auto const& pair) {
                     return pair.first;
                   });
    return keys;
  }

  std::vector<value> get_values() const {
    std::vector<typename Sequence<K>::value> values;
    values.reserve(size());
    std::transform(map_.cbegin(), map_.cend(), std::back_inserter(values),
                   [](auto const& pair) {
                     return pair.second;
                   });
    return values;
  }

protected:
  std::map<key, value> map_;

private:
  void insert_or_add_element(const std::pair<key, value>& element) {
      if (!map_.insert(element).second)
    map_.at(element.first) += element.second;
  }

};

#endif
