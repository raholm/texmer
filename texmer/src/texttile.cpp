#include "texttile.h"

#include <exception>

#include "util.h"

TextTile::TextTile(std::size_t sentence_size, std::size_t block_size,
                   const std::string& method, bool liberal)
  : transformer_{sentence_size}, identifier_{liberal}
{

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
  auto segments = segmenter_.segment(corpus, boundaries);

  return segments;
}

DocumentSegments TextTile::segment(const Document& doc,
                                   const Document& stopwords) const {
  auto doc_ts = transformer_.transform(doc, stopwords);
  auto scores = evaluator_->evaluate(doc_ts);
  auto boundaries = identifier_.get_boundaries(scores);
  auto segments = segmenter_.segment(doc, boundaries);

  return segments;
}
