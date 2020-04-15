#ifndef SIMULATION_RESULTS_H
#define SIMULATION_RESULTS_H

#include <vector>

class SimulationResult {
public:
  SimulationResult(double pericentre, double planetDistance,
                   double hillsRadius);
  ~SimulationResult();

  void updateCounts(bool bhBound, bool starBound);
  void updateAverages(double semiMajorBh, double semiMajorStar,
                      double eccentricityBh, double eccentricityStar);

  bool sameParameters(double pericentre, double planetDistance) const;

  double pericentre() const;
  double planetDistance() const;
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
  double m_pericentre;
  double m_planetDistance;
  double m_hillsRadius;
  double m_sumSemiMajorBh;
  double m_sumSemiMajorStar;
  double m_sumEccentricityBh;
  double m_sumEccentricityStar;
  std::size_t m_bhBoundCount;
  std::size_t m_starBoundCount;
  std::size_t m_totalCount;
};

#endif /* SIMULATION_RESULTS_H */
