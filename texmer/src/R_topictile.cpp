#include <Rcpp.h>

#include "def.h"
#include "util.h"
#include "topictile.h"
#include "type_topic_indicator_mode.h"

using namespace texmer;

void mode_update(TypeTopicIndicatorMode* mode,
                 const Rcpp::StringVector& tokens,
                 const Rcpp::IntegerVector& topic_indicators) {
  mode->update(Rcpp::as<StringVector>(tokens),
               Rcpp::as<IntVector>(topic_indicators));
}

int mode_get_mode(TypeTopicIndicatorMode* const mode,
                  const Rcpp::CharacterVector& token) {
  return mode->get_mode(Rcpp::as<TypeTopicIndicatorMode::type>(token));
}

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

Rcpp::List mode_get_topictile_segments_cpp(TypeTopicIndicatorMode* const modes,
                                           const Rcpp::List& tokens,
                                           const Rcpp::StringVector& stopwords,
                                           size_t sentence_size,
                                           size_t block_size,
                                           size_t n_segments,
                                           bool liberal) {
  TopicTile topictile{sentence_size, block_size, *modes,
      n_segments, liberal};
  auto segments = topictile.segment(convert_from_R(tokens),
                                    convert_from_R(stopwords));
  return convert_to_R(segments);
}

RCPP_MODULE(mod_topictile) {

  Rcpp::class_<TypeTopicIndicatorMode>("TypeTopicIndicatorMode")

    .constructor()

    .method("update", &mode_update)
    .method("get_mode", &mode_get_mode)
    .method("topictile", &mode_get_topictile_segments_cpp)
    ;
}
