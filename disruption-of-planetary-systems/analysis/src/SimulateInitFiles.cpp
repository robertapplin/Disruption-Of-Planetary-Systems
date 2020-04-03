#include "SimulateInitFiles.h"
#include "SimParameters.h"

#include "Logger.h"

#include <cstdlib>

namespace {

char *toChar(std::string const &str) {
  char *cstr = new char[str.length() + 1];
  strcpy(cstr, str.c_str());
  return cstr;
}

} // namespace

InitFileSimulator::InitFileSimulator(std::string const &drive,
                                     std::string const &subDirectory)
    : m_drive(drive), m_subDirectory(subDirectory),
      m_directory(m_drive + "/" + m_subDirectory) {}

InitFileSimulator::~InitFileSimulator() {}

void InitFileSimulator::simulateInitFiles(
    std::vector<SimParameters *> const &simulationParameters) {
  auto const cmd = toChar(getCommand(simulationParameters));
  system(cmd);
  deleteInitFiles(simulationParameters);
}

std::string InitFileSimulator::getCommand(
    std::vector<SimParameters *> const &simulationParameters) const {
  std::string cmd = m_drive + " && cd " + m_subDirectory + " && ";
  for (auto const &parameters : simulationParameters) {
    cmd += "NewARC.out <" + parameters->filename() + ".init> data.log";
    if (parameters != *(simulationParameters.end() - 1))
      cmd += " && ";
  }
  return std::move(cmd);
}

void InitFileSimulator::deleteInitFiles(
    std::vector<SimParameters *> const &simulationParameters) const {
  for (auto const &parameters : simulationParameters)
    deleteFile(parameters->filename() + ".init");
}

void InitFileSimulator::deleteFile(std::string const &filename) const {
  if (remove(toChar(m_directory + filename)) != 0) {
    Logger::getInstance().addLog(LogType::Warning,
                                 "Failed to delete file " + filename + ".");
  }
}
