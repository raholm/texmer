#ifndef TEXMER_BOUNDARY_IDENTIFIER_H_
#define TEXMER_BOUNDARY_IDENTIFIER_H_

#include "def.h"

namespace texmer {

  class BoundaryIdentifier {
  public:
    BoundaryIdentifier() = default;

    virtual ~BoundaryIdentifier() = default;

    virtual IntMatrix get_boundaries(const DoubleMatrix& scores) const = 0;
    virtual IntVector get_boundaries(const DoubleVector& scores) const = 0;

  };

  class TextTileBoundaryIdentifier : public BoundaryIdentifier {
  public:
    explicit TextTileBoundaryIdentifier(bool liberal)
      : liberal_{liberal} {}


    ~TextTileBoundaryIdentifier() = default;

    IntMatrix get_boundaries(const DoubleMatrix& scores) const override;
    IntVector get_boundaries(const DoubleVector& scores) const override;

  private:
    bool liberal_;

  };

  class TopicTileBoundaryIdentifier : public BoundaryIdentifier {
  public:
    explicit TopicTileBoundaryIdentifier(size_t n_boundaries, bool liberal)
      : n_boundaries_{n_boundaries},
        liberal_{liberal} {}


    ~TopicTileBoundaryIdentifier() = default;

    IntMatrix get_boundaries(const DoubleMatrix& scores) const override;
    IntVector get_boundaries(const DoubleVector& scores) const override;

  private:
    size_t n_boundaries_;
    bool liberal_;

    IntVector get_boundaries_by_heuristic(const DoubleVector& scores) const;
    IntVector get_boundaries_by_fixed_count(const DoubleVector& scores) const;

  };

} // namespace texmer

#endif // TEXMER_BOUNDARY_IDENTIFIER_H_
