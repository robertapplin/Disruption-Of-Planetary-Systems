#ifndef DPSINTERFACEMODEL_H
#define DPSINTERFACEMODEL_H

#include <memory>
#include <string>
#include <vector>

struct InitSimulationParams;

class DPSInterfacePresenter;
class InitFileGenerator;
class InitFileSimulator;
class OutFileProcessor;

class DPSInterfaceModel {

public:
  DPSInterfaceModel(DPSInterfacePresenter *presenter,
                    std::string const &directory);
  ~DPSInterfaceModel();

  void updateNumberOfBodies(std::size_t numberOfBodies);
  void updateUseDefaultHeaderParams(bool useDefaults);
  void updateTimeStep(double timeStep);
  void updateNumberOfTimeSteps(std::size_t numberOfTimeSteps);
  void updateTrueAnomaly(double trueAnomaly);

  void run(std::string const &pericentres, std::string const &planetDistancesA,
           std::string const &planetDistancesB,
           std::size_t numberOfOrientations);

private:
  void setupInitFileSimulator();

  bool validate(std::string const &pericentres,
                std::vector<std::string> const &planetDistancesA,
                std::vector<std::string> const &planetDistancesB) const;

  void runAll(std::vector<std::string> const &pericentres,
              std::vector<std::string> const &planetDistancesA,
              std::vector<std::string> const &planetDistancesB,
              std::size_t numberOfOrientation) const;

  bool generateInitFiles(std::vector<std::string> const &pericentres,
                         std::vector<std::string> const &planetDistancesA,
                         std::vector<std::string> const &planetDistancesB,
                         std::size_t numberOfOrientation) const;

  bool simulateInitFiles(
      std::vector<InitSimulationParams> const &simParameters) const;
  void
  processOutFiles(std::vector<InitSimulationParams> const &simParameters) const;

  template <typename Process>
  bool runProcess(Process const &predicate,
                  std::string const &processDescription) const;

  std::string m_directory;
  std::unique_ptr<InitFileGenerator> m_initFileGenerator;
  std::unique_ptr<InitFileSimulator> m_initFileSimulator;
  std::unique_ptr<OutFileProcessor> m_outFileProcessor;
  DPSInterfacePresenter *m_presenter;
};

#endif // !DPSINTERFACEMODEL_H
