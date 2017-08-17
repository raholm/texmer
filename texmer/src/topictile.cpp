#include "topictile.h"


namespace texmer {

  CorpusSegments TopicTile::segment(const Corpus& corpus, const StringVector& stopwords) const {
    auto transformed_corpus = transformer_.transform(corpus, stopwords, modes_);
    auto scores = evaluator_.evaluate(transformed_corpus);
    auto boundaries = identifier_.get_boundaries(scores);
    adjust_boundaries_by_sentence_size(boundaries);
    auto segments = segmenter_.segment(corpus, boundaries);
    return segments;
  }

  DocumentSegments TopicTile::segment(const Document& document, const StringVector& stopwords) const {
    auto transformed_document = transformer_.transform(document, stopwords, modes_);
    auto scores = evaluator_.evaluate(transformed_document);
    auto boundaries = identifier_.get_boundaries(scores);
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
