#ifndef BOUNDARY_IDENTIFIER_H
#define BOUNDARY_IDENTIFIER_H

#include "def.h"

class BoundaryIdentifier {
public:
  BoundaryIdentifier() = default;

  virtual ~BoundaryIdentifier() = default;

  virtual IntMatrix get_boundaries(const DoubleMatrix& scores) const = 0;
  virtual IntVector get_boundaries(const DoubleVector& scores) const = 0;

};

class TextTileBoundaryIdentifier : public BoundaryIdentifier {
public:
  TextTileBoundaryIdentifier(bool liberal);

  ~TextTileBoundaryIdentifier() = default;

  IntMatrix get_boundaries(const DoubleMatrix& scores) const override;
  IntVector get_boundaries(const DoubleVector& scores) const override;

private:
  bool liberal_;

};

class TopicTileBoundaryIdentifier : public BoundaryIdentifier {
public:
  TopicTileBoundaryIdentifier(std::size_t n_boundaries, bool liberal);

  ~TopicTileBoundaryIdentifier() = default;

  IntMatrix get_boundaries(const DoubleMatrix& scores) const override;
  IntVector get_boundaries(const DoubleVector& scores) const override;

private:
  std::size_t n_boundaries_;
  bool liberal_;

  IntVector get_boundaries_by_heuristic(const DoubleVector& scores) const;
  IntVector get_boundaries_by_fixed_count(const DoubleVector& scores) const;

};

#endif // BOUNDARY_IDENTIFIER_H
