#ifndef PROCESSOUTFILES_H
#define PROCESSOUTFILES_H

#include <fstream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

struct InitSimulationParams;
struct MultiPlanetResult;
struct SimulationResult;

class Body;
class MutableResult;
class TaskRunner;

class OutFileProcessor {

  enum PlanetID { None, A, B } const;

public:
  OutFileProcessor(std::string const &directory);
  ~OutFileProcessor();

  bool performAnalysis(
      std::vector<InitSimulationParams> const &simulationParameters);

private:
  void resetProcessor(std::size_t numberOfOutFiles);

  void processOutFiles(
      std::vector<InitSimulationParams> const &simulationParameters);
  void processOutFile(InitSimulationParams const &parameters);
  void processOutFile(InitSimulationParams const &parameters,
                      std::vector<std::unique_ptr<Body>> const &bodies);

  void computeSimulationResults4Body(InitSimulationParams const &parameters,
                                     Body const &blackHole, Body const &star,
                                     Body const &planetA, Body const &planetB);
  void computeSimulationResults(InitSimulationParams const &parameters,
                                Body const &blackHole, Body const &star,
                                Body const &planet, double planetDistance,
                                PlanetID const &planetID = PlanetID::None);

  std::vector<std::unique_ptr<Body>>
  loadOutFile(InitSimulationParams const &parameters) const;
  std::vector<std::unique_ptr<Body>>
  loadOutFile(std::ifstream &fileStream) const;
  std::vector<std::unique_ptr<Body>>
  loadOutFile3Body(std::ifstream &fileStream) const;
  std::vector<std::unique_ptr<Body>>
  loadOutFile4Body(std::ifstream &fileStream) const;

  std::vector<double>
  calculateBodyTotalEnergies(Body const &targetBody, Body const &otherBody,
                             std::size_t numberOfTimeSteps) const;
  double calculateTotalEnergy(Body const &targetBody, Body const &otherBody,
                              std::size_t index) const;

  bool isBound(Body const &targetBody, Body const &otherBody,
               std::size_t index) const;
  bool isBound(std::vector<double> const &energies) const;

  double calculateHillsRadius(double pericentre) const;

  std::pair<double, double> calculateOrbitalProperties(Body const &body1,
                                                       Body const &body2,
                                                       std::size_t index,
                                                       bool bound) const;
  double calculateSemiMajorAxis(Body const &body1, Body const &body2,
                                std::size_t index) const;
  double calculateEccentricity(Body const &body1, Body const &body2,
                               double semiMajorAxis, std::size_t index) const;

  void addResult(double pericentre, double planetDistance, bool bhBound,
                 bool starBound, double semiMajorBh, double semiMajorStar,
                 double eccentricityBh, double eccentricityStar,
                 PlanetID const &planetID);
  void addResult(std::map<std::pair<double, double>, MutableResult> &results,
                 double pericentre, double planetDistance, bool bhBound,
                 bool starBound, double semiMajorBh, double semiMajorStar,
                 double eccentricityBh, double eccentricityStar);
  bool updateResult(std::map<std::pair<double, double>, MutableResult> &results,
                    double pericentre, double planetDistance, bool bhBound,
                    bool starBound, double semiMajorBh, double semiMajorStar,
                    double eccentricityBh, double eccentricityStar);
  template <typename Predicate>
  bool updateResult(std::map<std::pair<double, double>, MutableResult> &results,
                    double pericentre, double planetDistance, bool bhBound,
                    bool starBound, double semiMajorBh, double semiMajorStar,
                    double eccentricityBh, double eccentricityStar,
                    Predicate const &predicate);

  void saveResults() const;
  void saveResults(std::string const &filename,
                   std::string const &fileText) const;
  void save3BodyResults() const;
  void save4BodyResults() const;

  std::map<std::pair<double, double>, MutableResult> combineResults() const;

  std::string generateResultsFileText(
      std::map<std::pair<double, double>, MutableResult> const &results) const;
  std::string
  generateResultFileLine(std::pair<double, double> const &parameters,
                         MutableResult const &result) const;

  std::mutex m_mutex;

  std::string m_directory;
  TaskRunner &m_taskRunner;

  std::map<std::pair<double, double>, MutableResult> m_resultsA;
  std::map<std::pair<double, double>, MutableResult> m_resultsB;
};

#endif /* PROCESSOUTFILES_H */
