#ifndef TOPIC_TILE_H
#define TOPIC_TILE_H

#include "def.h"
#include "type_topic_indicator_mode.h"
#include "topic_indicator_transformer.h"
#include "boundary_identifier.h"
#include "evaluator.h"
#include "token_segmenter.h"

class TopicTile {
public:
  TopicTile(std::size_t sentence_size, std::size_t block_size,
            const TypeTopicIndicatorMode& modes,
            std::size_t n_segments, bool liberal);

  ~TopicTile() = default;

  CorpusSegments segment(const Corpus& corpus, const StringVector& stopwords) const;
  DocumentSegments segment(const Document& document, const StringVector& stopwords) const;

private:
  std::size_t sentence_size_;
  TypeTopicIndicatorMode modes_;

  TopicIndicatorTransformer transformer_;
  TopicTileBlockEvaluator evaluator_;
  TopicTileBoundaryIdentifier identifier_;
  TokenSegmenter segmenter_;

  void adjust_boundaries_by_sentence_size(IntMatrix& boundaries) const;
  void adjust_boundaries_by_sentence_size(IntVector& boundaries) const;

};

#endif // TOPIC_TILE_H
