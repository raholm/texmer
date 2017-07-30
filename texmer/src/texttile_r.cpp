#include <Rcpp.h>

#include "texttile.h"
#include "util.h"

// [[Rcpp::export]]
Rcpp::List get_texttile_segments_cpp(const Rcpp::List& tokens,
                                     const Rcpp::StringVector& stopwords,
                                     std::size_t sentence_size,
                                     std::size_t block_size,
                                     const Rcpp::CharacterVector& method,
                                     bool liberal) {
  auto texttile = TextTile(sentence_size, block_size,
                           Rcpp::as<std::string>(method),
                           liberal);
  auto segments = texttile.segment(convert_from_R(tokens),
                                   convert_from_R(stopwords));
  return convert_to_R(segments);
}
