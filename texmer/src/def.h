#ifndef TEXMER_DEF_H_
#define TEXMER_DEF_H_

#include <string>
#include <vector>
#include <set>
#include <map>

namespace texmer {

  // General
  using size_t = std::size_t;
  using String = std::string;

  template<typename T>
  using Vector = std::vector<T>;

  template<typename T>
  using Set = std::set<T>;

  template<typename Key, typename Value>
  using Map = std::map<Key, Value>;

  template<typename First, typename Second>
  using Pair = std::pair<First, Second>;

  // Text
  using Token = String;
  using StringVector = Vector<Token>;

  using Document = StringVector;
  using Corpus = Vector<Document>;

  using Score = double;
  using DocumentScores = Vector<Score>;
  using CorpusScores = Vector<DocumentScores>;

  using DocumentSegments = Vector<String>;
  using CorpusSegments = Vector<DocumentSegments>;

  // Math
  using IntVector = Vector<size_t>;
  using DoubleVector = Vector<double>;
  using DoubleMatrix = Vector<DoubleVector>;
  using IntMatrix = Vector<IntVector>;

} // namespace texmer

#endif // TEXMER_DEF_H_
