#ifndef DPSINTERFACEMODEL_H
#define DPSINTERFACEMODEL_H

#include <memory>
#include <string>
#include <vector>

class InitFileGenerator;
class InitFileSimulator;
class OutFileProcessor;
class SimParameters;

class DPSInterfaceModel {

public:
  DPSInterfaceModel(std::string const &directory, double timeStep,
                    std::size_t numberOfTimeSteps, double trueAnomaly);
  ~DPSInterfaceModel();

  void updateTimeStep(double timeStep);
  void updateNumberOfTimeSteps(std::size_t numberOfTimeSteps);
  void updateTrueAnomaly(double trueAnomaly);

  void run(std::string const &pericentres, std::string const &planetDistances,
           std::size_t numberOfOrientations) const;

private:
  bool validate(std::string const &pericentres,
                std::string const &planetDistances) const;

  void run(std::vector<std::string> const &pericentres,
           std::vector<std::string> const &planetDistances,
           std::size_t numberOfOrientation) const;

  bool generateInitFiles(std::vector<std::string> const &pericentres,
                         std::vector<std::string> const &planetDistances,
                         std::size_t numberOfOrientation) const;

  bool
  simulateInitFiles(std::vector<SimParameters *> const &simParameters) const;
  void processOutFiles(std::vector<SimParameters *> const &simParameters) const;

  std::string m_directory;
  std::unique_ptr<InitFileGenerator> m_initFileGenerator;
  std::unique_ptr<InitFileSimulator> m_initFileSimulator;
  std::unique_ptr<OutFileProcessor> m_outFileProcessor;
};

#endif // !DPSINTERFACEMODEL_H
