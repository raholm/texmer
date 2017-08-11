#include "vocabulary.h"

namespace texmer {

  std::ostream& Vocabulary::print(std::ostream& out) const {
    out << "{";
    unsigned count = 0;
    for (auto const& token : vocabulary_) {
      out << token;
      if (++count != length())
        out << ", ";
    }
    out << "}";
    return out;
  }

} // namespace texmer
