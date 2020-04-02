#include "SimResults.h"

/*
  SimResult class methods
*/
SimResult::SimResult(double pericentre, double planetDistance, bool bhBound,
                     bool starBound, double semiMajorBh, double semiMajorStar,
                     double eccentricityBh, double eccentricityStar)
    : m_pericentre(pericentre), m_planetDistance(planetDistance),
      m_sumSemiMajorBh(semiMajorBh), m_sumSemiMajorStar(semiMajorStar),
      m_sumEccentricityBh(eccentricityBh),
      m_sumEccentricityStar(eccentricityStar), m_bhBoundCount(0),
      m_starBoundCount(0), m_totalCount(0) {
  updateCounts(bhBound, starBound);
}

SimResult::~SimResult() {}

void SimResult::updateCounts(bool bhBound, bool starBound) {
  if (bhBound)
    ++m_bhBoundCount;
  else if (starBound)
    ++m_starBoundCount;
  ++m_totalCount;
}

void SimResult::updateAverages(double semiMajorBh, double semiMajorStar,
                               double eccentricityBh, double eccentricityStar) {
  m_sumSemiMajorBh += semiMajorBh;
  m_sumSemiMajorStar += semiMajorStar;
  m_sumEccentricityBh += eccentricityBh;
  m_sumEccentricityStar += eccentricityStar;
}

bool SimResult::sameParameters(double pericentre, double planetDistance) const {
  return m_pericentre == pericentre && m_planetDistance == planetDistance;
}

double SimResult::pericentre() const { return m_pericentre; }

double SimResult::planetDistance() const { return m_planetDistance; }

double SimResult::bhBoundFraction() const {
  return static_cast<double>(m_bhBoundCount) /
         static_cast<double>(m_totalCount);
}

double SimResult::starBoundFraction() const {
  return static_cast<double>(m_starBoundCount) /
         static_cast<double>(m_totalCount);
}

double SimResult::unboundFraction() const {
  return 1.0 - bhBoundFraction() - starBoundFraction();
}

double SimResult::bhBoundFractionError() const {
  return sqrt(m_bhBoundCount) / m_totalCount;
}

double SimResult::starBoundFractionError() const {
  return sqrt(m_starBoundCount) / m_totalCount;
}

double SimResult::unboundFractionError() const {
  return sqrt(m_totalCount - m_bhBoundCount - m_starBoundCount) / m_totalCount;
}

double SimResult::semiMajorBh() const {
  return m_sumSemiMajorBh * bhBoundFraction();
}

double SimResult::semiMajorStar() const {
  return m_sumSemiMajorStar * starBoundFraction();
}

double SimResult::eccentricityBh() const {
  return m_sumEccentricityBh * bhBoundFraction();
}

double SimResult::eccentricityStar() const {
  return m_sumEccentricityStar * starBoundFraction();
}

/*
  SimResults class methods
*/
SimResults::SimResults() {}

SimResults::~SimResults() {}

void SimResults::addResult(double pericentre, double planetDistance,
                           bool bhBound, bool starBound, double semiMajorBh,
                           double semiMajorStar, double eccentricityBh,
                           double eccentricityStar) {
  for (auto const &result : m_results) {
    if (result->sameParameters(pericentre, planetDistance)) {
      result->updateCounts(bhBound, starBound);
      result->updateAverages(semiMajorBh, semiMajorStar, eccentricityBh,
                             eccentricityStar);
      return;
    }
  }
  m_results.emplace_back(new SimResult(pericentre, planetDistance, bhBound,
                                       starBound, semiMajorBh, semiMajorStar,
                                       eccentricityBh, eccentricityStar));
}

std::vector<SimResult *> SimResults::simulationResults() const {
  return m_results;
}
