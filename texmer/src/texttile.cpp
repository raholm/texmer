#include "texttile.h"

#include <exception>

#include "util.h"

TextTile::TextTile(std::size_t sentence_size, std::size_t block_size,
                   const std::string& method, bool liberal)
  : sentence_size_{sentence_size}, transformer_{sentence_size}, identifier_{liberal}
{
  if (sentence_size_ < 1) {
    throw std::invalid_argument("Invalid sentence size: '" + std::to_string(sentence_size_) + "'.");
  }

  if (to_lower(method) == "block")
    evaluator_ = new BlockEvaluator(block_size);
  else if (to_lower(method) == "vocabulary")
    evaluator_ = new VocabularyEvaluator(sentence_size);
  else
    throw std::invalid_argument("Invalid choice of method: '" + method + "'.");
}

TextTile::~TextTile() {
  delete evaluator_;
}

CorpusSegments TextTile::segment(const Corpus& corpus,
                                 const Document& stopwords) const {
  auto corpus_ts = transformer_.transform(corpus, stopwords);
  auto scores = evaluator_->evaluate(corpus_ts);
  auto boundaries = identifier_.get_boundaries(scores);
  adjust_boundaries_by_sentence_size(boundaries);
  auto segments = segmenter_.segment(corpus, boundaries);

  return segments;
}

DocumentSegments TextTile::segment(const Document& doc,
                                   const Document& stopwords) const {
  auto doc_ts = transformer_.transform(doc, stopwords);
  auto scores = evaluator_->evaluate(doc_ts);
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
  for (auto& boundary : boundaries)
    boundary *= sentence_size_;
}
