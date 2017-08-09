#include "token_sequence.h"

TokenSequence TokenSequence::operator-(const Vocabulary& rhs) const {
  TokenSequence tmp(*this);
  return tmp -= rhs;
}

TokenSequence& TokenSequence::operator-=(const Vocabulary& rhs) {
  for (auto it = map_.cbegin(); it != map_.cend();) {
    if (rhs.contains(it->first)) it = map_.erase(it);
    else ++it;
  }
  return *this;
}

Vocabulary TokenSequence::get_vocabulary() const {
  return Vocabulary(get_keys());
}
