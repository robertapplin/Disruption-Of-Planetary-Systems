#ifndef SIMRESULTS_H
#define SIMRESULTS_H

#include <vector>

/*
  SimResult class
*/
class SimResult {
public:
  SimResult(double pericentre, double planetDistance, bool bhBound,
            bool starBound, double semiMajorBh, double semiMajorStar,
            double eccentricityBh, double eccentricityStar);
  ~SimResult();

  void updateCounts(bool bhBound, bool starBound);
  void updateAverages(double semiMajorBh, double semiMajorStar,
                      double eccentricityBh, double eccentricityStar);

  bool sameParameters(double pericentre, double planetDistance) const;

  double pericentre() const;
  double planetDistance() const;

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
  double m_sumSemiMajorBh;
  double m_sumSemiMajorStar;
  double m_sumEccentricityBh;
  double m_sumEccentricityStar;
  std::size_t m_bhBoundCount;
  std::size_t m_starBoundCount;
  std::size_t m_totalCount;
};

/*
  SimResults class
*/
class SimResults {
public:
  SimResults();
  ~SimResults();

  void addResult(double pericentre, double planetDistance, bool bhBound,
                 bool starBound, double semiMajorBh, double semiMajorStar,
                 double eccentricityBh, double eccentricityStar);

  std::vector<SimResult *> simulationResults() const;

private:
  std::vector<SimResult *> m_results;
};

#endif /* SIMRESULTS_H */
