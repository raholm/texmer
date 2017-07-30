#ifndef TEXTTILE_H
#define TEXTTILE_H

#include "def.h"
#include "token_transformer.h"
#include "lexical_evaluator.h"
#include "boundary_identifier.h"
#include "token_segmenter.h"

class TextTile {
public:
  TextTile(std::size_t sentence_size, std::size_t block_size,
           const std::string& method, bool liberal);

  ~TextTile();

  CorpusSegments segment(const Corpus& tokens, const Document& stopwords) const;
  DocumentSegments segment(const Document& tokens, const Document& stopwords) const;

private:
  TokenTransformer transformer_;
  LexicalEvaluator* evaluator_;
  BoundaryIdentifier identifier_;
  TokenSegmenter segmenter_;

};

#endif // TEXTTILE_H
