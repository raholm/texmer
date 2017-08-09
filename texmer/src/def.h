#ifndef DEF_H
#define DEF_H

#include <string>
#include <vector>

using Token = std::string;

using Document = std::vector<Token>;
using Corpus = std::vector<Document>;

using Score = double;
using DocumentScores = std::vector<Score>;
using CorpusScores = std::vector<DocumentScores>;

using BoundaryPoints = std::vector<std::size_t>;

using DocumentSegments = std::vector<std::string>;
using CorpusSegments = std::vector<DocumentSegments>;

// Math
using DoubleMatrix = std::vector<std::vector<double>>;
using IntMatrix = std::vector<std::vector<std::size_t>>;
using DoubleVector = std::vector<double>;
using IntVector = std::vector<std::size_t>;

#endif // DEF_H
