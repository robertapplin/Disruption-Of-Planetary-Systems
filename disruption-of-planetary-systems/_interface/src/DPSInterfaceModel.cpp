#include "DPSInterfaceModel.h"
#include "GenerateInitFiles.h"
#include "ProcessOutFiles.h"
#include "SimParameters.h"
#include "SimulateInitFiles.h"

#include "Logger.h"
#include "PerformanceChecker.h"

#include <boost/algorithm/string.hpp>

static std::string const DRIVE = "D:";
static std::string const SUB_DIRECTORY = "FYP-Data";
static std::string const DIRECTORY = DRIVE + "\\" + SUB_DIRECTORY + "\\";

namespace {

std::vector<std::string> splitStringByDelimiter(std::string const &str,
                                                std::string const &delimiter) {
  std::vector<std::string> subStrings;
  boost::split(subStrings, str, boost::is_any_of(delimiter));
  return std::move(subStrings);
}

} // namespace

DPSInterfaceModel::DPSInterfaceModel()
    : m_initFileGenerator(std::make_unique<InitFileGenerator>(DIRECTORY)),
      m_initFileSimulator(
          std::make_unique<InitFileSimulator>(DRIVE, SUB_DIRECTORY)),
      m_outFileProcessor(std::make_unique<OutFileProcessor>(DIRECTORY)) {}

DPSInterfaceModel::~DPSInterfaceModel() {}

void DPSInterfaceModel::run(
    std::string const &pericentreString,
    std::string const &planetDistanceString,
    std::string const &numberOfOrientationString) const {
  generateInitFiles(pericentreString, planetDistanceString,
                    numberOfOrientationString);
  simulateInitFiles();
  processOutFiles();
}

void DPSInterfaceModel::generateInitFiles(
    std::string const &pericentreString,
    std::string const &planetDistanceString,
    std::string const &numberOfOrientationString) const {
  if (!pericentreString.empty() && !planetDistanceString.empty() &&
      !numberOfOrientationString.empty()) {
    generateInitFiles(splitStringByDelimiter(pericentreString, ","),
                      splitStringByDelimiter(planetDistanceString, ","),
                      std::stod(numberOfOrientationString));
  } else {
    Logger::getInstance().addLog(
        LogType::Error,
        "Invalid parameter provided: a parameter field is empty.");
  }
}

void DPSInterfaceModel::generateInitFiles(
    std::vector<std::string> const &pericentres,
    std::vector<std::string> const &planetDistances,
    std::size_t numberOfOrientation) const {
  auto &logger = Logger::getInstance();
  logger.addLog(LogType::Info, "Generating init files...");

  try {
    m_initFileGenerator->generateInitFiles(pericentres, planetDistances,
                                           numberOfOrientation);
    logger.addLog(LogType::Info, "Init files successfully generated.");
  } catch (std::runtime_error const &error) {
    logger.addLog(LogType::Error, error.what());
  }
}

void DPSInterfaceModel::simulateInitFiles() const {
  auto const simParameters = m_initFileGenerator->simulationParameters();
  m_initFileSimulator->simulateInitFiles(simParameters);
}

void DPSInterfaceModel::processOutFiles() const {
  auto timer = new TimeCheck();
  auto const simParameters = m_initFileGenerator->simulationParameters();
  m_outFileProcessor->processOutFiles(simParameters);
  timer->report();
}
