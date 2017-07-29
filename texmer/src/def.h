#ifndef DEF_H
#define DEF_H

#include <string>
#include <vector>

class TokenSequence;

using Token = std::string;

using Doc = std::vector<Token>;
using Corpus = std::vector<Doc>;
using Stopwords = Doc;
using Segment = Doc;

using DocTokenSequences = std::vector<TokenSequence>;
using CorpusTokenSequences = std::vector<DocTokenSequences>;
using BlockTokenSequences = DocTokenSequences;

using Score = double;
using DocScores = std::vector<Score>;
using CorpusScores = std::vector<DocScores>;

using BoundaryPoints = std::vector<std::size_t>;

using DocSegments = std::vector<std::size_t>;
using CorpusSegments = std::vector<DocSegments>;

#endif // DEF_H
