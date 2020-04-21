#ifndef SIMULATION_RESULTS_H
#define SIMULATION_RESULTS_H

#include <string>

struct SimulationResult {
  SimulationResult();
  SimulationResult(double hillsRadius, bool bhBound, bool starBound,
                   double semiMajorBh, double semiMajorStar,
                   double eccentricityBh, double eccentricityStar);
  SimulationResult(double hillsRadius, std::size_t bhBoundCount,
                   std::size_t starBoundCount, std::size_t totalCount,
                   double sumSemiMajorBh, double sumSemiMajorStar,
                   double sumEccentricityBh, double sumEccentricityStar);
  ~SimulationResult();

  double m_hillsRadius;
  double m_sumSemiMajorBh;
  double m_sumSemiMajorStar;
  double m_sumEccentricityBh;
  double m_sumEccentricityStar;
  std::size_t m_bhBoundCount;
  std::size_t m_starBoundCount;
  std::size_t m_totalCount;
};

class MutableResult {
public:
  MutableResult();
  MutableResult(double hillsRadius, bool bhBound, bool starBound,
                double semiMajorBh, double semiMajorStar, double eccentricityBh,
                double eccentricityStar);
  MutableResult(double hillsRadius, std::size_t bhBoundCount,
                std::size_t starBoundCount, std::size_t totalCount,
                double sumSemiMajorBh, double sumSemiMajorStar,
                double sumEccentricityBh, double sumEccentricityStar);
  ~MutableResult();

  MutableResult operator+(MutableResult const &otherResult) const;

  double hillsRadius() const;

  double bhBoundFraction() const;
  double starBoundFraction() const;
  double unboundFraction() const;

  double bhBoundFractionError() const;
  double starBoundFractionError() const;
  double unboundFractionError() const;

  double semiMajorBh() const;
  double semiMajorStar() const;
  double eccentricityBh() const;
  double eccentricityStar() const;

private:
  void updateCounts(bool bhBound, bool starBound);
  void updateAverages(double semiMajorBh, double semiMajorStar,
                      double eccentricityBh, double eccentricityStar);

  mutable SimulationResult m_result;
};

#endif /* SIMULATION_RESULTS_H */
