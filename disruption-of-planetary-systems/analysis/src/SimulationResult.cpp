#include "SimulationResult.h"

SimulationResult::SimulationResult() {}

SimulationResult::SimulationResult(double hillsRadius, bool bhBound,
                                   bool starBound, double semiMajorBh,
                                   double semiMajorStar, double eccentricityBh,
                                   double eccentricityStar)
    : m_hillsRadius(hillsRadius), m_bhBoundCount(0), m_starBoundCount(0),
      m_totalCount(0), m_sumSemiMajorBh(0), m_sumSemiMajorStar(0),
      m_sumEccentricityBh(0), m_sumEccentricityStar(0) {}

SimulationResult::SimulationResult(
    double hillsRadius, std::size_t bhBoundCount, std::size_t starBoundCount,
    std::size_t totalCount, double sumSemiMajorBh, double sumSemiMajorStar,
    double sumEccentricityBh, double sumEccentricityStar)
    : m_hillsRadius(hillsRadius), m_bhBoundCount(bhBoundCount),
      m_starBoundCount(starBoundCount), m_totalCount(totalCount),
      m_sumSemiMajorBh(sumSemiMajorBh), m_sumSemiMajorStar(sumSemiMajorStar),
      m_sumEccentricityBh(sumEccentricityBh),
      m_sumEccentricityStar(sumEccentricityStar) {}

SimulationResult::~SimulationResult() {}

MutableResult::MutableResult() {}

MutableResult::MutableResult(double hillsRadius, bool bhBound, bool starBound,
                             double semiMajorBh, double semiMajorStar,
                             double eccentricityBh, double eccentricityStar)
    : m_result(SimulationResult(hillsRadius, bhBound, starBound, semiMajorBh,
                                semiMajorStar, eccentricityBh,
                                eccentricityStar)) {
  updateCounts(bhBound, starBound);
  updateAverages(semiMajorBh, semiMajorStar, eccentricityBh, eccentricityStar);
}

MutableResult::MutableResult(double hillsRadius, std::size_t bhBoundCount,
                             std::size_t starBoundCount, std::size_t totalCount,
                             double sumSemiMajorBh, double sumSemiMajorStar,
                             double sumEccentricityBh,
                             double sumEccentricityStar)
    : m_result(SimulationResult(hillsRadius, bhBoundCount, starBoundCount,
                                totalCount, sumSemiMajorBh, sumSemiMajorStar,
                                sumEccentricityBh, sumEccentricityStar)) {}

MutableResult::~MutableResult() {}

MutableResult MutableResult::operator+(MutableResult const &otherResult) const {
  auto const other = otherResult.m_result;
  return MutableResult(
      m_result.m_hillsRadius, m_result.m_bhBoundCount + other.m_bhBoundCount,
      m_result.m_starBoundCount + other.m_starBoundCount,
      m_result.m_totalCount + other.m_totalCount,
      m_result.m_sumSemiMajorBh + other.m_sumSemiMajorBh,
      m_result.m_sumSemiMajorStar + other.m_sumSemiMajorStar,
      m_result.m_sumEccentricityBh + other.m_sumEccentricityBh,
      m_result.m_sumEccentricityStar + other.m_sumEccentricityStar);
}

void MutableResult::updateCounts(bool bhBound, bool starBound) {
  if (bhBound)
    ++m_result.m_bhBoundCount;
  else if (starBound)
    ++m_result.m_starBoundCount;
  ++m_result.m_totalCount;
}

void MutableResult::updateAverages(double semiMajorBh, double semiMajorStar,
                                   double eccentricityBh,
                                   double eccentricityStar) {
  m_result.m_sumSemiMajorBh += semiMajorBh;
  m_result.m_sumSemiMajorStar += semiMajorStar;
  m_result.m_sumEccentricityBh += eccentricityBh;
  m_result.m_sumEccentricityStar += eccentricityStar;
}

double MutableResult::hillsRadius() const { return m_result.m_hillsRadius; }

double MutableResult::bhBoundFraction() const {
  return static_cast<double>(m_result.m_bhBoundCount) /
         static_cast<double>(m_result.m_totalCount);
}

double MutableResult::starBoundFraction() const {
  return static_cast<double>(m_result.m_starBoundCount) /
         static_cast<double>(m_result.m_totalCount);
}

double MutableResult::unboundFraction() const {
  return 1.0 - bhBoundFraction() - starBoundFraction();
}

double MutableResult::bhBoundFractionError() const {
  return sqrt(m_result.m_bhBoundCount) / m_result.m_totalCount;
}

double MutableResult::starBoundFractionError() const {
  return sqrt(m_result.m_starBoundCount) / m_result.m_totalCount;
}

double MutableResult::unboundFractionError() const {
  return sqrt(m_result.m_totalCount - m_result.m_bhBoundCount -
              m_result.m_starBoundCount) /
         m_result.m_totalCount;
}

double MutableResult::semiMajorBh() const {
  return m_result.m_sumSemiMajorBh * bhBoundFraction();
}

double MutableResult::semiMajorStar() const {
  return m_result.m_sumSemiMajorStar * starBoundFraction();
}

double MutableResult::eccentricityBh() const {
  return m_result.m_sumEccentricityBh * bhBoundFraction();
}

double MutableResult::eccentricityStar() const {
  return m_result.m_sumEccentricityStar * starBoundFraction();
}
