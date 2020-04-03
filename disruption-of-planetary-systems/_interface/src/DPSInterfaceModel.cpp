#include "DPSInterfaceModel.h"
#include "GenerateInitFiles.h"
#include "ProcessOutFiles.h"
#include "SimParameters.h"
#include "SimulateInitFiles.h"

#include "Logger.h"
#include "PerformanceChecker.h"

#include <boost/algorithm/string.hpp>

namespace {

std::vector<std::string> splitStringByDelimiter(std::string const &str,
                                                std::string const &delimiter) {
  std::vector<std::string> subStrings;
  boost::split(subStrings, str, boost::is_any_of(delimiter));
  return std::move(subStrings);
}

} // namespace

DPSInterfaceModel::DPSInterfaceModel(std::string const &directory,
                                     double timeStep,
                                     std::size_t numberOfTimeSteps,
                                     double trueAnomaly)
    : m_directory(directory),
      m_initFileGenerator(std::make_unique<InitFileGenerator>(
          m_directory, timeStep, numberOfTimeSteps, trueAnomaly)),
      m_outFileProcessor(std::make_unique<OutFileProcessor>(m_directory)) {

  auto const split = splitStringByDelimiter(m_directory, ":");
  auto const drive = split[0] + ":";
  auto subDir = split[1];
  subDir.erase(0, 1);
  m_initFileSimulator = std::make_unique<InitFileSimulator>(drive, subDir);
}

DPSInterfaceModel::~DPSInterfaceModel() {}

bool DPSInterfaceModel::validate(std::string const &pericentres,
                                 std::string const &planetDistances) const {
  bool validInput(true);

  auto &logger = Logger::getInstance();
  if (pericentres.empty()) {
    validInput = false;
    logger.addLog(LogType::Warning, "Pericentre field is empty.");
  }
  if (planetDistances.empty()) {
    validInput = false;
    logger.addLog(LogType::Warning, "Planet distance field is empty.");
  }

  return validInput;
}

void DPSInterfaceModel::updateTimeStep(double timeStep) {
  m_initFileGenerator->setTimeStep(timeStep);
}

void DPSInterfaceModel::updateNumberOfTimeSteps(std::size_t numberOfTimeSteps) {
  m_initFileGenerator->setNumberOfTimeSteps(numberOfTimeSteps);
}

void DPSInterfaceModel::updateTrueAnomaly(double trueAnomaly) {
  m_initFileGenerator->setTrueAnomaly(trueAnomaly);
}

void DPSInterfaceModel::run(std::string const &pericentres,
                            std::string const &planetDistances,
                            std::size_t numberOfOrientations) const {
  if (validate(pericentres, planetDistances))
    run(splitStringByDelimiter(pericentres, ","),
        splitStringByDelimiter(planetDistances, ","), numberOfOrientations);
}

void DPSInterfaceModel::run(std::vector<std::string> const &pericentres,
                            std::vector<std::string> const &planetDistances,
                            std::size_t numberOfOrientation) const {
  if (generateInitFiles(pericentres, planetDistances, numberOfOrientation)) {
    auto const simParameters = m_initFileGenerator->simulationParameters();
    if (simulateInitFiles(simParameters))
      processOutFiles(simParameters);
  }
}

bool DPSInterfaceModel::generateInitFiles(
    std::vector<std::string> const &pericentres,
    std::vector<std::string> const &planetDistances,
    std::size_t numberOfOrientation) const {
  auto &logger = Logger::getInstance();
  logger.addLog(LogType::Info, "Generating init files...");

  try {
    auto timer = TimeCheck();
    m_initFileGenerator->generateInitFiles(pericentres, planetDistances,
                                           numberOfOrientation);
    logger.addLog(LogType::Info, "Generating init files: success (" +
                                     std::to_string(timer.timeElapsed()) +
                                     "s).");
    return true;
  } catch (std::runtime_error const &error) {
    logger.addLog(LogType::Error, error.what());
    return false;
  }
}

bool DPSInterfaceModel::simulateInitFiles(
    std::vector<SimParameters *> const &simParameters) const {
  auto &logger = Logger::getInstance();
  logger.addLog(LogType::Info, "Simulating init files...");

  try {
    auto timer = TimeCheck();
    m_initFileSimulator->simulateInitFiles(simParameters);
    logger.addLog(LogType::Info, "Simulating init files: success (" +
                                     std::to_string(timer.timeElapsed()) +
                                     "s).");
    return true;
  } catch (std::runtime_error const &error) {
    logger.addLog(LogType::Error, error.what());
    return false;
  }
}

void DPSInterfaceModel::processOutFiles(
    std::vector<SimParameters *> const &simParameters) const {
  auto &logger = Logger::getInstance();
  logger.addLog(LogType::Info, "Processing out files...");

  try {
    auto timer = TimeCheck();
    m_outFileProcessor->processOutFiles(simParameters);
    logger.addLog(LogType::Info, "Processing out files: success (" +
                                     std::to_string(timer.timeElapsed()) +
                                     "s).");
  } catch (std::runtime_error const &error) {
    logger.addLog(LogType::Error, error.what());
  }
}
