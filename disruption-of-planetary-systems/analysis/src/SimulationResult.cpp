#include "SimulationResult.h"

namespace {

std::vector<double> combineVectors(std::size_t combinedSize,
                                   std::vector<double> const &vec1,
                                   std::vector<double> const &vec2) {
  std::vector<double> combined;
  combined.reserve(combinedSize);
  combined.insert(combined.begin(), vec1.begin(), vec1.end());
  combined.insert(combined.begin(), vec2.begin(), vec2.end());
  return combined;
}

} // namespace

SimulationResult::SimulationResult() {}

SimulationResult::SimulationResult(double hillsRadius, bool bhBound,
                                   bool starBound, double semiMajorBh,
                                   double semiMajorStar, double eccentricityBh,
                                   double eccentricityStar)
    : m_hillsRadius(hillsRadius), m_bhBoundCount(0), m_starBoundCount(0),
      m_totalCount(0) {}

SimulationResult::SimulationResult(double hillsRadius, std::size_t bhBoundCount,
                                   std::size_t starBoundCount,
                                   std::size_t totalCount,
                                   std::vector<double> semiMajorsBh,
                                   std::vector<double> semiMajorsStar,
                                   std::vector<double> eccentricitiesBh,
                                   std::vector<double> eccentricitiesStar)
    : m_hillsRadius(hillsRadius), m_bhBoundCount(bhBoundCount),
      m_starBoundCount(starBoundCount), m_totalCount(totalCount),
      m_semiMajorsBh(semiMajorsBh), m_semiMajorsStar(semiMajorsStar),
      m_eccentricitiesBh(eccentricitiesBh),
      m_eccentricitiesStar(eccentricitiesStar) {}

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
                             std::vector<double> semiMajorsBh,
                             std::vector<double> semiMajorsStar,
                             std::vector<double> eccentricitiesBh,
                             std::vector<double> eccentricitiesStar)
    : m_result(SimulationResult(hillsRadius, bhBoundCount, starBoundCount,
                                totalCount, semiMajorsBh, semiMajorsStar,
                                eccentricitiesBh, eccentricitiesStar)) {}

MutableResult::~MutableResult() {}

MutableResult MutableResult::operator+(MutableResult const &otherResult) const {
  auto const other = otherResult.m_result;

  return MutableResult(
      m_result.m_hillsRadius, m_result.m_bhBoundCount + other.m_bhBoundCount,
      m_result.m_starBoundCount + other.m_starBoundCount,
      m_result.m_totalCount + other.m_totalCount,
      combineVectors(m_result.m_totalCount + other.m_totalCount,
                     m_result.m_semiMajorsBh, other.m_semiMajorsBh),
      combineVectors(m_result.m_totalCount + other.m_totalCount,
                     m_result.m_semiMajorsStar, other.m_semiMajorsStar),
      combineVectors(m_result.m_totalCount + other.m_totalCount,
                     m_result.m_eccentricitiesBh, other.m_eccentricitiesBh),
      combineVectors(m_result.m_totalCount + other.m_totalCount,
                     m_result.m_eccentricitiesStar,
                     other.m_eccentricitiesStar));
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
  if (semiMajorBh != 0.0) {
    m_result.m_semiMajorsBh.emplace_back(semiMajorBh);
    m_result.m_eccentricitiesBh.emplace_back(eccentricityBh);
  } else if (semiMajorStar != 0.0) {
    m_result.m_semiMajorsStar.emplace_back(semiMajorStar);
    m_result.m_eccentricitiesStar.emplace_back(eccentricityStar);
  }
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

std::vector<double> MutableResult::semiMajorsBh() const {
  return m_result.m_semiMajorsBh;
}

std::vector<double> MutableResult::semiMajorsStar() const {
  return m_result.m_semiMajorsStar;
}

std::vector<double> MutableResult::eccentricitiesBh() const {
  return m_result.m_eccentricitiesBh;
}

std::vector<double> MutableResult::eccentricitiesStar() const {
  return m_result.m_eccentricitiesStar;
}
