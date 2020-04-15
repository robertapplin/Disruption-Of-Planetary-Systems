#ifndef PROCESSOUTFILES_H
#define PROCESSOUTFILES_H

#include <memory>
#include <mutex>
#include <string>
#include <utility>
#include <vector>

struct InitSimulationParams;

class Body;
class SimulationResult;
class TaskRunner;

class OutFileProcessor {
public:
  OutFileProcessor(std::string const &directory);
  ~OutFileProcessor();

  bool processOutFiles(
      std::vector<InitSimulationParams> const &simulationParameters);

private:
  void resetProcessor(std::size_t numberOfOutFiles);

  void processOutFile(InitSimulationParams const &parameters);

  void computeSimulationResults4Body(InitSimulationParams const &parameters,
                                     Body const &blackHole, Body const &star,
                                     Body const &planetA, Body const &planetB);
  void computeSimulationResults(InitSimulationParams const &parameters,
                                Body const &blackHole, Body const &star,
                                Body const &planet, double planetDistance);

  std::vector<std::unique_ptr<Body>>
  loadOutFile(InitSimulationParams const &parameters) const;
  std::vector<std::unique_ptr<Body>>
  loadOutFile3Body(std::string const &filename) const;
  std::vector<std::unique_ptr<Body>>
  loadOutFile4Body(std::string const &filename) const;

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

  void saveResults() const;
  std::string generateResultsFileText() const;
  std::string generateResultFileLine(SimulationResult const &result) const;

  void initializeResults(
      std::vector<InitSimulationParams> const &simulationParameters);
  void initializeResult(double pericentre, double planetDistance);
  bool haveResult(double pericentre, double planetDistance) const;
  void addResult(double pericentre, double planetDistance, bool bhBound,
                 bool starBound, double semiMajorBh, double semiMajorStar,
                 double eccentricityBh, double eccentricityStar);

  std::mutex m_mutex;

  std::string m_directory;
  TaskRunner &m_taskRunner;
  std::vector<std::unique_ptr<SimulationResult>> m_results;
};

#endif /* PROCESSOUTFILES_H */
