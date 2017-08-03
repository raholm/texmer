#ifndef TOKEN_SEGMENTER_H
#define TOKEN_SEGMENTER_H

#include "def.h"

class TokenSegmenter {
public:
  TokenSegmenter() = default;

  ~TokenSegmenter() = default;

  CorpusSegments segment(const Corpus& tokens,
                         const IntMatrix& boundaries) const;

  DocumentSegments segment(const Document& tokens,
                           const IntVector& boundaries) const;

private:
  IntVector get_segment_ids(const IntVector& boundaries,
                            std::size_t n_tokens) const;
  void fill_with_segment_ids(IntVector& segment_ids, std::size_t& from,
                             int with, unsigned times) const;

};

#endif // TOKEN_SEGMENTER_H
