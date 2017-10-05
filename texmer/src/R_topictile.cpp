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

Rcpp::IntegerVector mode_types_to_topic_indicators(TypeTopicIndicatorMode* const mode,
                                                   const Rcpp::StringVector& types) {
  IntVector topic_indicators;
  topic_indicators.reserve(types.size());

  for (auto const& t : types) {
    auto type = Rcpp::as<TypeTopicIndicatorMode::type>(t);
    if (mode->contains(type))
      topic_indicators.push_back(mode->get_mode(type));
  }

  return Rcpp::wrap(topic_indicators);
}

Rcpp::DataFrame mode_get_data(TypeTopicIndicatorMode* const mode) {
  auto data = mode->get_data();

  Rcpp::StringVector types(data.size());
  Rcpp::IntegerVector topic_indicators(data.size());

  for (unsigned i = 0; i < data.size(); ++i) {
    auto pair = data.at(i);
    types(i) = pair.first;
    topic_indicators(i) = pair.second;
  }

  return Rcpp::DataFrame::create(Rcpp::Named("type") = types,
                                 Rcpp::Named("topic_indicator") = topic_indicators);
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
                                           bool liberal,
                                           size_t smoothing_rounds,
                                           size_t smoothing_width) {
  TopicTile topictile(sentence_size,
                      block_size,
                      *modes,
                      n_segments,
                      liberal,
                      smoothing_rounds,
                      smoothing_width);
  auto segments = topictile.segment(convert_from_R(tokens),
                                    convert_from_R(stopwords));
  return convert_to_R(segments);
}

RCPP_MODULE(mod_topictile) {

  Rcpp::class_<TypeTopicIndicatorMode>("TypeTopicIndicatorMode")

    .constructor()

    .method("update", &mode_update)
    .method("types_to_topic_indicators", &mode_types_to_topic_indicators)
    .method("data", &mode_get_data)
    .method("topictile", &mode_get_topictile_segments_cpp)
    ;
}
