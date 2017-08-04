#ifndef TEXTTILE_H
#define TEXTTILE_H

#include "def.h"
#include "token_transformer.h"
#include "lexical_evaluator.h"
#include "boundary_identifier.h"
#include "token_segmenter.h"

#include <memory>

class TextTile {
public:
  TextTile(std::size_t sentence_size, std::size_t block_size,
           const std::string& method, bool liberal);

  ~TextTile() = default;

  CorpusSegments segment(const Corpus& tokens, const Document& stopwords) const;
  DocumentSegments segment(const Document& tokens, const Document& stopwords) const;

private:
  std::size_t sentence_size_;

  TokenTransformer transformer_;
  std::unique_ptr<LexicalEvaluator> evaluator_;
  TextTileBoundaryIdentifier identifier_;
  TokenSegmenter segmenter_;

  void adjust_boundaries_by_sentence_size(IntMatrix& boundaries) const;
  void adjust_boundaries_by_sentence_size(IntVector& boundaries) const;

  TextTile() = delete;
  TextTile(const TextTile& other) = delete;
  TextTile(TextTile&& other) = delete;

  TextTile& operator=(const TextTile& rhs) = delete;
  TextTile& operator=(TextTile&& rhs) = delete;

};

#endif // TEXTTILE_H
