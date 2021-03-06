#include <Rcpp.h>

#include "def.h"
#include "util.h"
#include "texttile.h"

using namespace texmer;

inline Corpus convert_from_R(const Rcpp::List& corpus) {
  Corpus c;

  for (auto const& document : corpus) {
    c.push_back(Rcpp::as<Document>(document));
  }

  return c;
}

inline Document convert_from_R(const Rcpp::StringVector& document) {
  return Rcpp::as<Document>(document);
}

inline Rcpp::StringVector convert_to_R(const DocumentSegments& segments) {
  return Rcpp::wrap(segments);
}

inline Rcpp::List convert_to_R(const CorpusSegments& segments) {
  return Rcpp::wrap(segments);
}

// [[Rcpp::export]]
Rcpp::List get_texttile_segments_cpp(const Rcpp::List& tokens,
                                     const Rcpp::StringVector& stopwords,
                                     size_t sentence_size,
                                     size_t block_size,
                                     const Rcpp::CharacterVector& method,
                                     bool liberal,
                                     size_t smoothing_rounds,
                                     size_t smoothing_width) {
  TextTile texttile(sentence_size,
                    block_size,
                    Rcpp::as<std::string>(method),
                    liberal,
                    smoothing_rounds,
                    smoothing_width);
  auto segments = texttile.segment(convert_from_R(tokens),
                                   convert_from_R(stopwords));
  return convert_to_R(segments);
}
