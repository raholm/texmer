#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include "sequence.h"

#include <algorithm>
#include <numeric>

template<typename K>
Sequence<K>::Sequence(const std::vector<K>& keys) {
  for (auto const& key : keys)
    insert_or_add_element(std::make_pair(key, 1));
}

template<typename K>
Sequence<K> Sequence<K>::operator+(const Sequence& rhs) const {
  Sequence<K> tmp(*this);
  return tmp += rhs;
}

template<typename K>
Sequence<K>& Sequence<K>::operator+=(const Sequence& rhs) {
  for (auto const &p : rhs.map_) {
    insert_or_add_element(p);
  }
  return *this;
}

template<typename K>
Sequence<K> Sequence<K>::operator*(const Sequence& rhs) const {
  Sequence<K> tmp(*this);
  return tmp *= rhs;
}

template<typename K>
Sequence<K>& Sequence<K>::operator*=(const Sequence& rhs) {
  for (auto it = map_.cbegin(); it != map_.cend();) {
    auto rit = rhs.map_.find(it->first);
    if (rit != rhs.map_.end()) {
      map_.at(it->first) = it->second * rit->second;
      it++;
    } else it = map_.erase(it);
  }
  return *this;
}

template<typename K>
bool Sequence<K>::operator==(const Sequence& rhs) const {
  return size() == rhs.size() &&
    std::equal(map_.cbegin(), map_.cend(), rhs.map_.cbegin());
}

template<typename K>
bool Sequence<K>::operator!=(const Sequence& rhs) const {
  return !(*this == rhs);
}

template<typename K>
std::size_t Sequence<K>::size() const {
  return map_.size();
}

template<typename K>
std::size_t Sequence<K>::length() const {
  return std::accumulate(map_.cbegin(), map_.cend(), 0,
                         [](const std::size_t acc,
                            auto const& p) {
                           return acc + p.second;
                         });
}

template<typename K>
std::vector<K> Sequence<K>::get_keys() const {
  std::vector<typename Sequence<K>::key> keys;
  keys.reserve(size());
  std::transform(map_.cbegin(), map_.cend(), std::back_inserter(keys),
                 [](auto const& pair) {
                   return pair.first;
                 });
  return keys;
}

template<typename K>
std::vector<typename Sequence<K>::value> Sequence<K>::get_values() const {
  std::vector<typename Sequence<K>::value> values;
  values.reserve(size());
  std::transform(map_.cbegin(), map_.cend(), std::back_inserter(values),
                 [](auto const& pair) {
                   return pair.second;
                 });
  return values;
}

template<typename K>
void Sequence<K>::insert_or_add_element(const std::pair<
                                        typename Sequence<K>::key,
                                        typename Sequence<K>::value>& element) {
  if (!map_.insert(element).second)
    map_.at(element.first) += element.second;
}

#endif // SEQUENCE_HPP
