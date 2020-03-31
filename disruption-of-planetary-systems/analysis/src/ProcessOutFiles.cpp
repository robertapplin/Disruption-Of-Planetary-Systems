#include "ProcessOutFiles.h"
#include "SimParameters.h"

#include "FileManager.h"
#include "Logger.h"

#include <cstdio>
#include <fstream>
#include <streambuf>

namespace {

char *toChar(std::string const &str) {
  char *cstr = new char[str.length() + 1];
  strcpy(cstr, str.c_str());
  return cstr;
}

} // namespace

OutFileProcessor::OutFileProcessor(std::string const &directory)
    : m_directory(directory) {}

OutFileProcessor::~OutFileProcessor() {}

void OutFileProcessor::processOutFiles(
    std::vector<SimParameters *> const &simulationParameters) const {

  for (auto const &parameters : simulationParameters) {

    // auto outData =
    processOutFile(parameters->filename());

    deleteInitFile(parameters->filename());
  }
}

void OutFileProcessor::processOutFile(std::string const &filename) const {

  auto outData = loadOutFile(filename);
}

std::string OutFileProcessor::loadOutFile(std::string const &filename) const {
  // std::ifstream infile(m_directory + filename + ".out");
  // double a, bhMass, bhX, bhY, bhZ, bhVx, bhVy, bhVz, sMass, sX, sY, sZ, sVx,
  //    sVy, sVz, pMass, pX, pY, pZ, pVx, pVy, pVz;
  // while (infile >> a >> bhMass >> bhX >> bhY >> bhZ >> bhVx >> bhVy >> bhVz
  // >>
  //       sMass >> sX >> sY >> sZ >> sVx >> sVy >> sVz >> pMass >> pX >> pY >>
  //       pZ >> pVx >> pVy >> pVz) {
  // bhXs.emplace_back();
  // bhYs.emplace_back();
  // bhZs.emplace_back();
  // bhVxs.emplace_back();
  // bhVys.emplace_back();
  // bhVzs.emplace_back();

  //}
  // return std::vector<Body *>{createBody(), createBody(), createBody()};
  return "";
}

std::string
OutFileProcessor::configureOutData(std::string const &rawData) const {
  return rawData;
}

void OutFileProcessor::deleteInitFile(std::string const &filename) const {
  if (!remove(toChar(m_directory + filename + ".init"))) {
    Logger::getInstance().addLog(LogType::Warning, "Failed to delete file " +
                                                       filename + ".init.");
  }
}
