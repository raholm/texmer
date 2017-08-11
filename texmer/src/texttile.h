#ifndef TEXMER_TEXTTILE_H_
#define TEXMER_TEXTTILE_H_

#include <memory>

#include "def.h"
#include "token_transformer.h"
#include "lexical_evaluator.h"
#include "boundary_identifier.h"
#include "token_segmenter.h"

namespace texmer {

  class TextTile {
  public:
    explicit TextTile(size_t sentence_size, size_t block_size,
                      const String& method, bool liberal);

    ~TextTile();

    CorpusSegments segment(const Corpus& tokens, const Document& stopwords) const;
    DocumentSegments segment(const Document& tokens, const Document& stopwords) const;

  private:
    size_t sentence_size_;

    TokenTransformer transformer_;
    LexicalEvaluatorBase* evaluator_;
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

} // namespace texmer

#endif // TEXMER_TEXTTILE_H_
