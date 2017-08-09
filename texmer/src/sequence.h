#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <vector>
#include <map>

template<typename K>
class Sequence {
public:
  using key = K;
  using value = std::size_t;

  Sequence() = default;
  Sequence(const Sequence& other) = default;
  Sequence(Sequence&& other) = default;
  Sequence(const std::vector<key>& keys);

  virtual ~Sequence() = default;

  Sequence& operator=(const Sequence& other) = default;
  Sequence& operator=(Sequence&& other) = default;

  Sequence operator+(const Sequence& rhs) const;
  Sequence& operator+=(const Sequence& rhs);

  Sequence operator*(const Sequence& rhs) const;
  Sequence& operator*=(const Sequence& rhs);

  bool operator==(const Sequence& rhs) const;
  bool operator!=(const Sequence& rhs) const;

  std::size_t size() const;
  std::size_t length() const;

  std::vector<key> get_keys() const;
  std::vector<value> get_values() const;

protected:
  std::map<key, value> map_;

private:
  void insert_or_add_element(const std::pair<key, value>& element);

};

#include "sequence.hpp"

#endif // SEQUENCE_H
