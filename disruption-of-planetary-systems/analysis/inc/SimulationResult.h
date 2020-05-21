#ifndef SIMULATION_RESULTS_H
#define SIMULATION_RESULTS_H

#include <string>
#include <vector>

struct SimulationResult {
  SimulationResult();
  SimulationResult(double hillsRadius, bool bhBound, bool starBound,
                   double semiMajorBh, double semiMajorStar,
                   double eccentricityBh, double eccentricityStar);
  SimulationResult(double hillsRadius, std::size_t bhBoundCount,
                   std::size_t starBoundCount, std::size_t totalCount,
                   std::vector<double> semiMajorsBh,
                   std::vector<double> semiMajorsStar,
                   std::vector<double> eccentricitiesBh,
                   std::vector<double> eccentricitiesStar);
  ~SimulationResult();

  double m_hillsRadius;
  std::size_t m_bhBoundCount;
  std::size_t m_starBoundCount;
  std::size_t m_totalCount;
  std::vector<double> m_semiMajorsBh;
  std::vector<double> m_semiMajorsStar;
  std::vector<double> m_eccentricitiesBh;
  std::vector<double> m_eccentricitiesStar;
};

class MutableResult {
public:
  MutableResult();
  MutableResult(double hillsRadius, bool bhBound, bool starBound,
                double semiMajorBh, double semiMajorStar, double eccentricityBh,
                double eccentricityStar);
  MutableResult(double hillsRadius, std::size_t bhBoundCount,
                std::size_t starBoundCount, std::size_t totalCount,
                std::vector<double> semiMajorsBh,
                std::vector<double> semiMajorsStar,
                std::vector<double> eccentricitiesBh,
                std::vector<double> eccentricitiesStar);
  ~MutableResult();

  MutableResult operator+(MutableResult const &otherResult) const;

  double hillsRadius() const;

  double bhBoundFraction() const;
  double starBoundFraction() const;
  double unboundFraction() const;

  double bhBoundFractionError() const;
  double starBoundFractionError() const;
  double unboundFractionError() const;

  std::vector<double> semiMajorsBh() const;
  std::vector<double> semiMajorsStar() const;
  std::vector<double> eccentricitiesBh() const;
  std::vector<double> eccentricitiesStar() const;

private:
  void updateCounts(bool bhBound, bool starBound);
  void updateAverages(double semiMajorBh, double semiMajorStar,
                      double eccentricityBh, double eccentricityStar);

  mutable SimulationResult m_result;
};

#endif /* SIMULATION_RESULTS_H */
