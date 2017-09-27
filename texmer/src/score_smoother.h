#ifndef TEXMER_SCORE_SMOOTHER_H_
#define TEXMER_SCORE_SMOOTHER_H_

#include <exception>

#include "def.h"

namespace texmer {

  class ScoreSmoother {
  public:
    virtual ~ScoreSmoother() = default;

    virtual CorpusScores smooth(const CorpusScores& scores) const = 0;
    virtual DocumentScores smooth(const DocumentScores& scores) const = 0;

  };

  class AverageScoreSmoother : public ScoreSmoother {
  public:
    explicit AverageScoreSmoother(size_t rounds, size_t width)
      : rounds_{rounds},
        width_{width}
    {
      if (width_ % 2 != 0)
        throw std::invalid_argument("Width must be even.");
    }

    CorpusScores smooth(const CorpusScores& scores) const override {
      CorpusScores result;
      result.reserve(scores.size());

      for (auto const& s : scores)
        result.push_back(smooth(s));

      return result;
    }

    DocumentScores smooth(const DocumentScores& scores) const override {
      DocumentScores result{scores};
      DocumentScores tmp{scores};

      size_t lower, upper;
      size_t width2 = width_ / 2;
      double val;

      for (unsigned i = 0; i < rounds_; ++i) {
        for (unsigned current_gap = 0; current_gap < result.size(); ++current_gap) {
          lower = std::max(0, (int) (current_gap - width2));
          upper = std::min(result.size() - 1, current_gap + width2);

          if (lower == upper) continue;

          val = 0;
          for (unsigned j = lower; j <= upper; ++j)
            val += result.at(j);

          tmp.at(current_gap) = val / (1 + upper - lower);
        }

        result = tmp;
      }

      return result;
    }

  private:
    size_t rounds_;
    size_t width_;

  };

}

#endif // TEXMER_SCORE_SMOOTHER_H_
