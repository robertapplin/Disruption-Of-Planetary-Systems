#include "SimulateInitFiles.h"
#include "InitSimulationParams.h"

#include "FileManager.h"
#include "Logger.h"
#include "TaskRunner.h"

#include <cstdlib>
#include <math.h>

namespace {

char *toChar(std::string const &str) {
  char *cstr = new char[str.length() + 1];
  strcpy_s(cstr, str.length() + 1, str.c_str());
  return cstr;
}

std::size_t calculateNumberOfIntermissions(std::size_t numberOfSimulations,
                                           std::size_t stepSize) {
  return static_cast<std::size_t>(
      std::ceil(static_cast<double>(numberOfSimulations) /
                static_cast<double>(stepSize)));
}

} // namespace

InitFileSimulator::InitFileSimulator(std::string const &drive,
                                     std::string const &subDirectory)
    : m_drive(drive), m_subDirectory(subDirectory),
      m_directory(m_drive + "/" + m_subDirectory),
      m_taskRunner(TaskRunner::getInstance()) {
  m_fileManager =
      std::make_unique<FileManager>(m_directory + "run_simulation.sh");
}

InitFileSimulator::~InitFileSimulator() {}

void InitFileSimulator::resetSimulator(std::size_t numberOfIntermissions) {
  m_taskRunner.setTask("Simulating init files...", 10.0, 20.0);
  m_taskRunner.setNumberOfSteps(numberOfIntermissions);
}

bool InitFileSimulator::simulateInitFiles(
    std::vector<InitSimulationParams> const &simulationParameters) {
  auto const numberOfSimulations = simulationParameters.size();
  auto const numberOfIntermissions =
      calculateNumberOfIntermissions(numberOfSimulations, m_step);
  auto const remainder = numberOfSimulations % m_step;

  return simulateInitFiles(simulationParameters, numberOfIntermissions,
                           remainder);
}

bool InitFileSimulator::simulateInitFiles(
    std::vector<InitSimulationParams> const &simulationParameters,
    std::size_t numberOfIntermissions, std::size_t remainder) {
  resetSimulator(numberOfIntermissions);

  for (auto i = 0u; i < numberOfIntermissions; ++i) {
    if (m_taskRunner.isRunning())
      simulateInitFiles(simulationParameters.begin(), numberOfIntermissions,
                        remainder, i);
    else
      return false;
  }

  deleteInitFiles(simulationParameters);
  return true;
}

void InitFileSimulator::simulateInitFiles(
    std::vector<InitSimulationParams>::const_iterator const &beginIter,
    std::size_t numberOfIntermissions, std::size_t remainder,
    std::size_t index) const {
  if (index != numberOfIntermissions - 1 || remainder == 0)
    simulateInitFiles(beginIter + index * m_step,
                      beginIter + (index + 1) * m_step);
  else
    simulateInitFiles(beginIter + index * m_step,
                      beginIter + index * m_step + remainder);
}

void InitFileSimulator::simulateInitFiles(
    std::vector<InitSimulationParams>::const_iterator const &startIter,
    std::vector<InitSimulationParams>::const_iterator const &endIter) const {
  m_fileManager->createNewFile(getCommand(startIter, endIter));

  auto const cmd =
      m_drive + " && cd " + m_subDirectory + " && run_simulation.sh";
  system(toChar(cmd));

  m_taskRunner.reportProgress();
}

std::string InitFileSimulator::getCommand(
    std::vector<InitSimulationParams>::const_iterator const &startIter,
    std::vector<InitSimulationParams>::const_iterator const &endIter) const {
  std::string cmd;
  for (auto it = startIter; it < endIter; ++it) {
    cmd += "./NewARC.out <" + it->m_filename + ".init> data.log";
    if (*it != *(endIter - 1))
      cmd += "\n";
  }
  return std::move(cmd);
}

void InitFileSimulator::deleteInitFiles(
    std::vector<InitSimulationParams> const &simulationParameters) const {
  for (auto const &parameters : simulationParameters)
    deleteFile(parameters.m_filename + ".init");
}

void InitFileSimulator::deleteFile(std::string const &filename) const {
  if (remove(toChar(m_directory + filename)) != 0) {
    Logger::getInstance().addLog(LogType::Warning,
                                 "Failed to delete file " + filename + ".");
  }
}
