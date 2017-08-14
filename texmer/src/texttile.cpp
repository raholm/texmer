#include "texttile.h"

#include <exception>

#include "util.h"

namespace texmer {

  TextTile::TextTile(size_t sentence_size, size_t block_size,
                     const String& method, bool liberal)
    : sentence_size_{sentence_size},
      transformer_{sentence_size},
      identifier_{liberal} {
        if (sentence_size_ < 1) {
          throw std::invalid_argument("Invalid sentence size: '" + std::to_string(sentence_size_) + "'.");
        }

        if (to_lower(method) == "block")
          evaluator_ = new TextTileBlockEvaluator(block_size);
        else if (to_lower(method) == "vocabulary")
          evaluator_ = new TextTileVocabularyEvaluator(sentence_size);
        else
          throw std::invalid_argument("Invalid choice of method: '" + method + "'.");
      }

  TextTile::~TextTile() {
    delete evaluator_;
  }

  CorpusSegments TextTile::segment(const Corpus& corpus,
                                   const Document& stopwords) const {
    auto transformed_corpus = transformer_.transform(corpus, stopwords);

    CorpusScores scores;
    if (TextTileBlockEvaluator* e = dynamic_cast<TextTileBlockEvaluator*>(evaluator_))
      scores = e->evaluate(transformed_corpus);
    else if (TextTileVocabularyEvaluator* e = dynamic_cast<TextTileVocabularyEvaluator*>(evaluator_))
      scores = e->evaluate(transformed_corpus);

    auto boundaries = identifier_.get_boundaries(scores);
    adjust_boundaries_by_sentence_size(boundaries);
    auto segments = segmenter_.segment(corpus, boundaries);

    return segments;
  }

  DocumentSegments TextTile::segment(const Document& doc,
                                     const Document& stopwords) const {
    auto doc_ts = transformer_.transform(doc, stopwords);

    DocumentScores scores;
    if (TextTileBlockEvaluator* e = dynamic_cast<TextTileBlockEvaluator*>(evaluator_))
      scores = e->evaluate(doc_ts);
    else if (TextTileVocabularyEvaluator* e = dynamic_cast<TextTileVocabularyEvaluator*>(evaluator_))
      scores = e->evaluate(doc_ts);

    auto boundaries = identifier_.get_boundaries(scores);
    adjust_boundaries_by_sentence_size(boundaries);
    auto segments = segmenter_.segment(doc, boundaries);

    return segments;
  }

  void TextTile::adjust_boundaries_by_sentence_size(IntMatrix& boundaries) const {
    for (auto& _boundaries : boundaries)
      adjust_boundaries_by_sentence_size(_boundaries);

  }
  void TextTile::adjust_boundaries_by_sentence_size(IntVector& boundaries) const {
    for (auto& boundary : boundaries) {
      ++boundary;
      boundary *= sentence_size_;
      --boundary;
    }
  }

} // namespace texmer
