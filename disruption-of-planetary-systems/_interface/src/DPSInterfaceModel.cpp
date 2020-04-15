#include "DPSInterfaceModel.h"
#include "DPSInterfacePresenter.h"
#include "GenerateInitFiles.h"
#include "InitSimulationParams.h"
#include "ProcessOutFiles.h"
#include "SimulateInitFiles.h"

#include "Logger.h"
#include "PerformanceChecker.h"
#include "TaskRunner.h"

#include <boost/algorithm/string.hpp>

namespace {

std::vector<std::string> splitStringByDelimiter(std::string const &str,
                                                std::string const &delimiter) {
  std::vector<std::string> subStrings;
  boost::split(subStrings, str, boost::is_any_of(delimiter));
  return std::move(subStrings);
}

} // namespace

DPSInterfaceModel::DPSInterfaceModel(DPSInterfacePresenter *presenter,
                                     std::string const &directory)
    : m_presenter(presenter), m_directory(directory),
      m_initFileGenerator(std::make_unique<InitFileGenerator>(m_directory)),
      m_outFileProcessor(std::make_unique<OutFileProcessor>(m_directory)) {
  setupInitFileSimulator();
}

DPSInterfaceModel::~DPSInterfaceModel() {}

void DPSInterfaceModel::setupInitFileSimulator() {
  auto const split = splitStringByDelimiter(m_directory, ":");
  auto const drive = split[0] + ":";
  auto subDir = split[1];
  subDir.erase(0, 1);
  m_initFileSimulator = std::make_unique<InitFileSimulator>(drive, subDir);
}

void DPSInterfaceModel::updateNumberOfBodies(std::size_t numberOfBodies) {
  InitHeaderData::m_fixedHeaderParams->m_numberOfBodies = numberOfBodies;
}

void DPSInterfaceModel::updateUseDefaultHeaderParams(bool useDefaults) {
  InitHeaderData::m_useDefaults = useDefaults;
}

void DPSInterfaceModel::updateTimeStep(double timeStep) {
  InitHeaderData::m_fixedHeaderParams->m_timeStep = timeStep;
}

void DPSInterfaceModel::updateNumberOfTimeSteps(std::size_t numberOfTimeSteps) {
  InitHeaderData::m_fixedHeaderParams->m_numberOfTimeSteps = numberOfTimeSteps;
}

void DPSInterfaceModel::updateTrueAnomaly(double trueAnomaly) {
  InitHeaderData::m_fixedHeaderParams->m_trueAnomaly = trueAnomaly;
}

bool DPSInterfaceModel::validate(
    std::string const &pericentres,
    std::vector<std::string> const &planetDistancesA,
    std::vector<std::string> const &planetDistancesB) const {
  bool validInput(true);

  auto &logger = Logger::getInstance();
  if (pericentres.empty()) {
    validInput = false;
    logger.addLog(LogType::Warning, "Pericentre field is empty.");
  }
  if (planetDistancesA.empty()) {
    validInput = false;
    logger.addLog(LogType::Warning, "Planet distance field is empty.");
  }
  if (InitHeaderData::m_fixedHeaderParams->m_numberOfBodies == 4 &&
      planetDistancesA.size() != planetDistancesB.size()) {
    validInput = false;
    logger.addLog(LogType::Warning,
                  "Planet distance fields are not equal in size.");
  }

  return validInput;
}

void DPSInterfaceModel::run(std::string const &pericentres,
                            std::string const &planetDistancesA,
                            std::string const &planetDistancesB,
                            std::size_t numberOfOrientations) {
  auto const planetDistASplit = splitStringByDelimiter(planetDistancesA, ",");
  auto const planetDistBSplit = splitStringByDelimiter(planetDistancesB, ",");
  if (validate(pericentres, planetDistASplit, planetDistBSplit)) {
    runAll(splitStringByDelimiter(pericentres, ","), planetDistASplit,
           planetDistBSplit, numberOfOrientations);
  }
  m_presenter->unlockRunning();
}

void DPSInterfaceModel::runAll(std::vector<std::string> const &pericentres,
                               std::vector<std::string> const &planetDistancesA,
                               std::vector<std::string> const &planetDistancesB,
                               std::size_t numberOfOrientation) const {
  if (generateInitFiles(pericentres, planetDistancesA, planetDistancesB,
                        numberOfOrientation)) {
    auto const simParameters = m_initFileGenerator->simulationParameters();
    if (simulateInitFiles(simParameters))
      processOutFiles(simParameters);
  }
}

bool DPSInterfaceModel::generateInitFiles(
    std::vector<std::string> const &pericentres,
    std::vector<std::string> const &planetDistancesA,
    std::vector<std::string> const &planetDistancesB,
    std::size_t numberOfOrientation) const {
  auto const generateProcess = [&]() {
    return m_initFileGenerator->generate(pericentres, planetDistancesA,
                                         planetDistancesB, numberOfOrientation);
  };
  return runProcess(generateProcess, "Generating init files");
}

bool DPSInterfaceModel::simulateInitFiles(
    std::vector<InitSimulationParams> const &simParameters) const {
  auto const simulationProcess = [&]() {
    return m_initFileSimulator->simulateInitFiles(simParameters);
  };
  return runProcess(simulationProcess, "Simulating init files");
}

void DPSInterfaceModel::processOutFiles(
    std::vector<InitSimulationParams> const &simParameters) const {
  auto const dataAnalysisProcess = [&]() {
    return m_outFileProcessor->processOutFiles(simParameters);
  };
  (void)runProcess(dataAnalysisProcess, "Processing out files");
}

template <typename Process>
bool DPSInterfaceModel::runProcess(
    Process const &process, std::string const &processDescription) const {
  try {
    return process();
  } catch (std::runtime_error const &error) {
    Logger::getInstance().addLog(
        LogType::Error, processDescription + " failed: " + error.what());
    return false;
  }
}
