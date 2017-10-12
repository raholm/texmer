#include "boundary_identifier.h"

#include <algorithm>

#include "util.h"

namespace texmer {

  static double get_depth_cutoff(const DoubleVector& scores, bool liberal) {
    double avg = mean(scores);
    double stdev = sd(scores);

    if (liberal) return avg - stdev;
    else return avg - stdev / 2;
  }

  static double get_depth_by_side(const DoubleVector& scores, unsigned gap, bool left) {
    double depth_score = 0.0;
    unsigned current_gap = gap;

    while ((scores.at(current_gap) - scores.at(gap)) >= depth_score) {
      depth_score = scores.at(current_gap) - scores.at(gap);

      if (left) current_gap--;
      else current_gap++;

      if (current_gap < 0 || current_gap >= scores.size()) break;
    }

    return depth_score;
  }

  IntMatrix TextTileBoundaryIdentifier::get_boundaries(const DoubleMatrix& scores) const {
    IntMatrix boundaries;
    boundaries.reserve(scores.size());

    for (auto const& score : scores) {
      boundaries.push_back(get_boundaries(score));
    }

    return boundaries;
  }

  IntVector TextTileBoundaryIdentifier::get_boundaries(const DoubleVector& scores) const {
    IntVector boundaries;

    double cutoff_score = get_depth_cutoff(scores, liberal_);
    double depth_score, left_depth_score, right_depth_score;

    for (unsigned gap = 0; gap < scores.size(); ++gap) {
      left_depth_score = get_depth_by_side(scores, gap, true);
      right_depth_score = get_depth_by_side(scores, gap, false);

      depth_score = left_depth_score + right_depth_score;

      if (depth_score >= cutoff_score)
        boundaries.push_back(gap);
    }

    return boundaries;
  }

  IntMatrix TopicTileBoundaryIdentifier::get_boundaries(const DoubleMatrix& scores) const {
    IntMatrix boundaries;
    boundaries.reserve(scores.size());

    for (auto const& score : scores)
      boundaries.push_back(get_boundaries(score));

    return boundaries;
  }

  IntVector TopicTileBoundaryIdentifier::get_boundaries(const DoubleVector& scores) const {
    if (n_boundaries_ < 0) return get_boundaries_by_heuristic(scores);
    else return get_boundaries_by_fixed_count(scores);
  }

  IntVector TopicTileBoundaryIdentifier::get_boundaries_by_heuristic(const DoubleVector& scores) const {
    IntVector boundaries;

    double cutoff_score = get_depth_cutoff(scores, liberal_);
    double depth_score, left_depth_score, right_depth_score;

    for (unsigned gap = 0; gap < scores.size(); ++gap) {
      left_depth_score = get_depth_by_side(scores, gap, true);
      right_depth_score = get_depth_by_side(scores, gap, false);

      depth_score = left_depth_score + right_depth_score;

      if (depth_score >= cutoff_score)
        boundaries.push_back(gap);
    }

    return boundaries;
  }

  IntVector TopicTileBoundaryIdentifier::get_boundaries_by_fixed_count(const DoubleVector& scores) const {
    using GapDepthPair = Pair<size_t, double>;

    IntVector boundaries;
    boundaries.reserve(n_boundaries_);

    if (n_boundaries_ == 0)
      return boundaries;

    Vector<GapDepthPair> gap_depth;
    double depth_score, left_depth_score, right_depth_score;

    for (unsigned gap = 0; gap < scores.size(); ++gap) {
      left_depth_score = get_depth_by_side(scores, gap, true);
      right_depth_score = get_depth_by_side(scores, gap, false);

      depth_score = (left_depth_score + right_depth_score) / 2;

      gap_depth.push_back(std::make_pair(gap, depth_score));
    }

    std::sort(gap_depth.begin(), gap_depth.end(),
              [](GapDepthPair const& p1, GapDepthPair const& p2) {
                return p1.second > p2.second;
              });

    std::transform(gap_depth.cbegin(),
                   gap_depth.cbegin() + std::min((size_t) n_boundaries_, scores.size()),
                   std::back_inserter(boundaries),
                   [](GapDepthPair const& p) { return p.first; });

    std::sort(boundaries.begin(), boundaries.end());

    return boundaries;
  }

} // namespace texmer
