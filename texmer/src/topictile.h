#ifndef TEXMER_TOPIC_TILE_H_
#define TEXMER_TOPIC_TILE_H_

#include <exception>
#include <string>

#include "def.h"
#include "type_topic_indicator_mode.h"
#include "topic_indicator_transformer.h"
#include "boundary_identifier.h"
#include "lexical_evaluator.h"
#include "token_segmenter.h"
#include "score_smoother.h"

namespace texmer {

  class TopicTile {
  public:
    TopicTile(size_t sentence_size, size_t block_size,
              const TypeTopicIndicatorMode& modes,
              size_t n_segments, bool liberal,
              size_t smoothing_rounds=0,
              size_t smoothing_width=2);

    ~TopicTile() = default;

    CorpusSegments segment(const Corpus& corpus, const StringVector& stopwords) const;
    DocumentSegments segment(const Document& document, const StringVector& stopwords) const;

  private:
    size_t sentence_size_;
    TypeTopicIndicatorMode modes_;
    TopicIndicatorTransformer transformer_;
    TopicTileBlockEvaluator evaluator_;
    TopicTileBoundaryIdentifier identifier_;
    TokenSegmenter segmenter_;
    AverageScoreSmoother smoother_;

    void adjust_boundaries_by_sentence_size(IntMatrix& boundaries) const;
    void adjust_boundaries_by_sentence_size(IntVector& boundaries) const;

  };

} // namespace texmer

#endif // TEXMER_TOPIC_TILE_H_
