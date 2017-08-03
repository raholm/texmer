#include "boundary_identifier.h"

#include "util.h"

BoundaryIdentifier::BoundaryIdentifier(bool liberal)
  : liberal_(liberal)
{

}

IntMatrix BoundaryIdentifier::get_boundaries(const DoubleMatrix& scores) const {
  IntMatrix boundaries;

  for (auto const& score : scores) {
    boundaries.push_back(get_boundaries(score));
  }

  return boundaries;
}

IntVector BoundaryIdentifier::get_boundaries(const DoubleVector& scores) const {
  IntVector boundaries;

  double cutoff_score = get_depth_cutoff(scores);
  double depth_score, left_depth_score, right_depth_score;

  for (unsigned gap = 0; gap < scores.size(); ++gap) {
    left_depth_score = get_depth_by_side(scores, gap, true);
    right_depth_score = get_depth_by_side(scores, gap, false);

    depth_score = left_depth_score + right_depth_score;

    if (depth_score >= cutoff_score)
      boundaries.push_back(gap + 1);
  }

  return boundaries;
}

double BoundaryIdentifier::get_depth_cutoff(const DoubleVector& scores) const {
  double avg = mean(scores);
  double stdev = sd(scores);

  if (liberal_) return avg - stdev;
  else return avg - stdev / 2;
}

double BoundaryIdentifier::get_depth_by_side(const DoubleVector& scores,
                                             unsigned gap, bool left) const {
  double depth_score = 0;
  unsigned current_gap = gap;

  while ((scores.at(current_gap) - scores.at(gap)) >= depth_score) {
    depth_score = scores.at(current_gap) - scores.at(gap);

    if (left) --current_gap;
    else ++current_gap;

    if (current_gap < 0 || current_gap >= scores.size()) break;
  }

  return depth_score;
}
