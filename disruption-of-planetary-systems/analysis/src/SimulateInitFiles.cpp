#include "SimulateInitFiles.h"
#include "SimParameters.h"

#include "Logger.h"
#include "PerformanceChecker.h"

#include <cstdlib> // system()

namespace {

char *toChar(std::string const &str) {
  char *cstr = new char[str.length() + 1];
  strcpy(cstr, str.c_str());
  return cstr;
}

} // namespace

InitFileSimulator::InitFileSimulator(std::string const &drive,
                                     std::string const &subDirectory)
    : m_drive(drive), m_subDirectory(subDirectory) {}

InitFileSimulator::~InitFileSimulator() {}

void InitFileSimulator::simulateInitFiles(
    std::vector<SimParameters *> const &simulationParameters) {
  Logger::getInstance().addLog(LogType::Info, "Simulating init files...");
  auto timer = new TimeCheck();

  simulateFiles(simulationParameters);

  Logger::getInstance().addLog(
      LogType::Info, "Simulations successful (" +
                         std::to_string(timer->timeElapsed()) + "s).");
}

void InitFileSimulator::simulateFiles(
    std::vector<SimParameters *> const &simulationParameters) {
  std::string cmd = m_drive + " && cd " + m_subDirectory + " && ";
  for (auto const &parameters : simulationParameters) {
    cmd += "NewARC.out <" + parameters->filename() + ".init> data.log";
    if (parameters != *(simulationParameters.end() - 1))
      cmd += " && ";
  }
  system(toChar(cmd));
}
