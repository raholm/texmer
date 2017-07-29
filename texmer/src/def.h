#ifndef DEF_H
#define DEF_H

#include <string>
#include <vector>

#include "token_sequence.h"

using Token = std::string;

using Doc = std::vector<Token>;
using RDoc = Rcpp::StringVector;

using Corpus = std::vector<Doc>;
using RCorpus = Rcpp::List;

using Stopwords = Doc;
using RStopwords = RDoc;

using DocTokenSequences = std::vector<TokenSequence>;
using CorpusTokenSequences = std::vector<DocTokenSequences>;
using BlockTokenSequences = DocTokenSequences;

using Score = double;
using DocScores = std::vector<Score>;
using CorpusScores = std::vector<DocScores>;

using BoundaryPoints = std::vector<std::size_t>;

#endif // DEF_H
