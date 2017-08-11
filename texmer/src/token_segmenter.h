#ifndef TEXMER_TOKEN_SEGMENTER_H_
#define TEXMER_TOKEN_SEGMENTER_H_

#include "def.h"

namespace texmer {

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
                              size_t n_tokens) const;
    void fill_with_segment_ids(IntVector& segment_ids, size_t& from,
                               int with, unsigned times) const;
    void check_boundaries(const IntVector& boundaries, size_t limit) const;

  };

} // namespace texmer

#endif // TEXMER_TOKEN_SEGMENTER_H_
