#ifndef PROCESSOUTFILES_H
#define PROCESSOUTFILES_H

#include <string>
#include <utility>
#include <vector>

class Body;
class SimParameters;
class SimResult;
class SimResults;

class OutFileProcessor {
public:
  OutFileProcessor(std::string const &directory);
  ~OutFileProcessor();

  void processOutFiles(
      std::vector<SimParameters *> const &simulationParameters) const;

private:
  void processOutFile(SimParameters const &parameters,
                      SimResults &results) const;
  std::vector<Body *> loadOutFile(std::string const &filename) const;

  std::vector<double>
  calculateBodyTotalEnergies(Body const &targetBody, Body const &otherBody,
                             std::size_t numberOfTimeSteps) const;
  double calculateTotalEnergy(Body const &targetBody, Body const &otherBody,
                              std::size_t index) const;

  bool isBound(std::vector<double> const &energies) const;

  double calculateSemiMajorAxis(Body const &body1, Body const &body2,
                                std::size_t index) const;
  double calculateEccentricity(Body const &body1, Body const &body2,
                               double semiMajorAxis, std::size_t index) const;

  void deleteInitFile(std::string const &filename) const;

  void saveResults(SimResults const &results) const;
  std::string generateFileText(SimResults const &results) const;
  std::string generateFileLine(SimResult const *result) const;

  std::string m_directory;
};

#endif /* PROCESSOUTFILES_H */
