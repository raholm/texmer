#include "token_segmenter.h"

#include <stdexcept>

CorpusSegments TokenSegmenter::segment(const Corpus& tokens,
                                       const IntMatrix& boundaries) const {
  std::size_t n = boundaries.size();
  CorpusSegments segments(n);

  for (unsigned i = 0; i < n; ++i) {
    segments.at(i) = segment(tokens.at(i), boundaries.at(i));
  }

  return segments;
}

DocumentSegments TokenSegmenter::segment(const Document& tokens,
                                         const IntVector& boundaries) const {
  if (tokens.size() == 0)
    return DocumentSegments();

  check_boundaries(boundaries, tokens.size());

  IntVector segment_ids = get_segment_ids(boundaries, tokens.size());
  DocumentSegments segments;

  std::size_t current_id = segment_ids.at(0);
  std::size_t previous_id = current_id;
  std::string current_segment{tokens.at(0)};

  for (unsigned i = 1; i < segment_ids.size(); ++i) {
    current_id = segment_ids.at(i);

    if (current_id != previous_id) {
      segments.push_back(current_segment);
      current_segment = tokens.at(i);
    } else {
      current_segment += " " + tokens.at(i);
    }

    previous_id = current_id;
  }

  // Add the last segment
  segments.push_back(current_segment);

  return segments;
}

IntVector TokenSegmenter::get_segment_ids(const IntVector& boundaries,
                                          std::size_t n_tokens) const {
  if (boundaries.size() == 0)
    return IntVector(n_tokens, 1);

  IntVector segment_ids(n_tokens);

  std::size_t n_segs = boundaries.size() + 1;
  std::size_t id_idx = 0;

  for (unsigned seg_id = 0; seg_id < n_segs; ++seg_id) {
    if (seg_id == 0) {
      fill_with_segment_ids(segment_ids, id_idx, seg_id + 1,
                            boundaries.at(seg_id) + 1);
    } else if (seg_id == (n_segs - 1)) {
      fill_with_segment_ids(segment_ids, id_idx, seg_id + 1,
                            n_tokens - (boundaries.at(seg_id - 1) + 1));
    } else {
      fill_with_segment_ids(segment_ids, id_idx, seg_id + 1,
                            boundaries.at(seg_id) - boundaries.at(seg_id - 1));
    }
  }

  return segment_ids;
}

void TokenSegmenter::fill_with_segment_ids(IntVector& segment_ids, std::size_t& from,
                                           int with, unsigned times) const {
  for (unsigned i = 0; i < times; ++i) {
    segment_ids.at(from++) = with;
  }
}

void TokenSegmenter::check_boundaries(const IntVector& boundaries, std::size_t limit) const {
  for (auto const& boundary : boundaries)
    if (boundary < 0 || boundary >= limit)
      throw std::out_of_range("Boundary point is out of range.");
}
