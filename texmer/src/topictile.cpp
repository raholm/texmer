#include "topictile.h"

namespace texmer {

  TopicTile::TopicTile(size_t sentence_size, size_t block_size,
                       const TypeTopicIndicatorMode& modes,
                       size_t n_segments, bool liberal,
                       size_t smoothing_rounds,
                       size_t smoothing_width)
    : sentence_size_{sentence_size},
      modes_{modes},
      transformer_{sentence_size},
      evaluator_{block_size},
      identifier_{(int) n_segments - 1, liberal},
      segmenter_{},
      smoother_{smoothing_rounds, smoothing_width}
  {
    if (sentence_size_ < 1) {
      throw std::invalid_argument("Invalid sentence size: '" + std::to_string(sentence_size_) + "'.");
    }
  }

  CorpusSegments TopicTile::segment(const Corpus& corpus, const StringVector& stopwords) const {
    auto transformed_corpus = transformer_.transform(corpus, stopwords, modes_);
    auto scores = evaluator_.evaluate(transformed_corpus);
    auto boundaries = identifier_.get_boundaries(smoother_.smooth(scores));
    adjust_boundaries_by_sentence_size(boundaries);
    auto segments = segmenter_.segment(corpus, boundaries);
    return segments;
  }

  DocumentSegments TopicTile::segment(const Document& document, const StringVector& stopwords) const {
    auto transformed_document = transformer_.transform(document, stopwords, modes_);
    auto scores = evaluator_.evaluate(transformed_document);
    auto boundaries = identifier_.get_boundaries(smoother_.smooth(scores));
    adjust_boundaries_by_sentence_size(boundaries);
    auto segments = segmenter_.segment(document, boundaries);
    return segments;
  }

  void TopicTile::adjust_boundaries_by_sentence_size(IntMatrix& boundaries) const {
    for (auto& _boundaries : boundaries)
      adjust_boundaries_by_sentence_size(_boundaries);

  }
  void TopicTile::adjust_boundaries_by_sentence_size(IntVector& boundaries) const {
    for (auto& boundary : boundaries) {
      ++boundary;
      boundary *= sentence_size_;
      --boundary;
    }
  }

} // namespace texmer
