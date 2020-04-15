#include "SimulationResult.h"

SimulationResult::SimulationResult(double pericentre, double planetDistance,
                                   double hillsRadius)
    : m_pericentre(pericentre), m_planetDistance(planetDistance),
      m_hillsRadius(hillsRadius), m_sumSemiMajorBh(0), m_sumSemiMajorStar(0),
      m_sumEccentricityBh(0), m_sumEccentricityStar(0), m_bhBoundCount(0),
      m_starBoundCount(0), m_totalCount(0) {}

SimulationResult::~SimulationResult() {}

void SimulationResult::updateCounts(bool bhBound, bool starBound) {
  if (bhBound)
    ++m_bhBoundCount;
  else if (starBound)
    ++m_starBoundCount;
  ++m_totalCount;
}

void SimulationResult::updateAverages(double semiMajorBh, double semiMajorStar,
                                      double eccentricityBh,
                                      double eccentricityStar) {
  m_sumSemiMajorBh += semiMajorBh;
  m_sumSemiMajorStar += semiMajorStar;
  m_sumEccentricityBh += eccentricityBh;
  m_sumEccentricityStar += eccentricityStar;
}

bool SimulationResult::sameParameters(double pericentre,
                                      double planetDistance) const {
  return m_pericentre == pericentre && m_planetDistance == planetDistance;
}

double SimulationResult::pericentre() const { return m_pericentre; }

double SimulationResult::planetDistance() const { return m_planetDistance; }

double SimulationResult::hillsRadius() const { return m_hillsRadius; }

double SimulationResult::bhBoundFraction() const {
  return static_cast<double>(m_bhBoundCount) /
         static_cast<double>(m_totalCount);
}

double SimulationResult::starBoundFraction() const {
  return static_cast<double>(m_starBoundCount) /
         static_cast<double>(m_totalCount);
}

double SimulationResult::unboundFraction() const {
  return 1.0 - bhBoundFraction() - starBoundFraction();
}

double SimulationResult::bhBoundFractionError() const {
  return sqrt(m_bhBoundCount) / m_totalCount;
}

double SimulationResult::starBoundFractionError() const {
  return sqrt(m_starBoundCount) / m_totalCount;
}

double SimulationResult::unboundFractionError() const {
  return sqrt(m_totalCount - m_bhBoundCount - m_starBoundCount) / m_totalCount;
}

double SimulationResult::semiMajorBh() const {
  return m_sumSemiMajorBh * bhBoundFraction();
}

double SimulationResult::semiMajorStar() const {
  return m_sumSemiMajorStar * starBoundFraction();
}

double SimulationResult::eccentricityBh() const {
  return m_sumEccentricityBh * bhBoundFraction();
}

double SimulationResult::eccentricityStar() const {
  return m_sumEccentricityStar * starBoundFraction();
}
