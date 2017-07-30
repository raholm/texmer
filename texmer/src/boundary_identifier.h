#ifndef BOUNDARY_IDENTIFIER_H
#define BOUNDARY_IDENTIFIER_H

#include "def.h"

class BoundaryIdentifier {
public:
  BoundaryIdentifier(bool liberal);

  ~BoundaryIdentifier() = default;

  IntMatrix get_boundaries(const DoubleMatrix& scores) const;
  IntVector get_boundaries(const DoubleVector& scores) const;

private:
  bool liberal_;

  double get_depth_cutoff(const DoubleVector& scores) const;
  double get_depth_by_side(const DoubleVector& scores,
                           unsigned gap, bool left) const;
};

#endif // BOUNDARY_IDENTIFIER_H
