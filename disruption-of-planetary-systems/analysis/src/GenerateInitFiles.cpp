#include "GenerateInitFiles.h"

#include "Body.h"
#include "BodyCreator.h"
#include "InitSimulationParams.h"

#include "FileManager.h"
#include "TaskRunner.h"

namespace {

using namespace BodyCreator;

std::size_t randomNumber(int lower, int higher) {
  return static_cast<std::size_t>(rand() % (higher - lower) + lower);
}

std::string formatSimParameter(double value) {
  return std::to_string(static_cast<int>(value)) + ".0";
}

void generateFileLine(std::string &fileText, double mass,
                      XYZComponents const &position,
                      XYZComponents const &velocity) {
  fileText += "\n  " + std::to_string(mass) + "   " +
              std::to_string(position.compX()) + "   " +
              std::to_string(position.compY()) + "   " +
              std::to_string(position.compZ()) + "   " +
              std::to_string(velocity.compX()) + "   " +
              std::to_string(velocity.compY()) + "   " +
              std::to_string(velocity.compZ());
}

void generateFileText(std::string &fileText) { (void)(fileText); }

template <typename Body, typename... Bodies>
void generateFileText(std::string &fileText, Body const &body,
                      Bodies const &... bodies) {
  auto const position = body.position(BodyTime::Initial);
  auto const velocity = body.velocity(BodyTime::Initial);
  generateFileLine(fileText, body.mass(), position, velocity);
  generateFileText(fileText, bodies...);
}

} // namespace

InitFileGenerator::InitFileGenerator(std::string const &directory)
    : m_fileManager(std::make_unique<FileManager>()), m_directory(directory),
      m_taskRunner(TaskRunner::getInstance()) {}

InitFileGenerator::~InitFileGenerator() {}

void InitFileGenerator::resetGenerator(std::size_t numberOfInitFiles) {
  resetInitSimulationParams(numberOfInitFiles);
  m_taskRunner.setTask("Generating init files...", 0.0, 10.0);
  m_taskRunner.setNumberOfSteps(numberOfInitFiles);
}

void InitFileGenerator::resetInitSimulationParams(
    std::size_t numberOfInitFiles) {
  m_simulationParams.clear();
  m_simulationParams.reserve(numberOfInitFiles);
}

void InitFileGenerator::addInitSimulationParams(
    InitSimulationParams const &simParameters) {
  m_simulationParams.emplace_back(simParameters);
}

std::vector<InitSimulationParams> const &
InitFileGenerator::simulationParameters() const {
  return m_simulationParams;
}

void InitFileGenerator::createFile(std::string const &filename,
                                   std::string const &fileText) const {
  m_fileManager->setFilename(m_directory + filename);
  m_fileManager->createNewFile(fileText);
}

bool InitFileGenerator::generate(
    std::vector<std::string> const &pericentres,
    std::vector<std::string> const &planetDistancesA,
    std::vector<std::string> const &planetDistancesB,
    std::size_t numberOfOrientations) {
  resetGenerator(pericentres.size() * planetDistancesA.size() *
                 numberOfOrientations);
  return generateInitFiles(pericentres, planetDistancesA, planetDistancesB,
                           numberOfOrientations);
}

bool InitFileGenerator::generateInitFiles(
    std::vector<std::string> const &pericentres,
    std::vector<std::string> const &planetDistancesA,
    std::vector<std::string> const &planetDistancesB,
    std::size_t numberOfOrientations) {
  for (auto const &pericentre : pericentres) {
    for (auto i = 0u; i < planetDistancesA.size(); ++i) {
      if (m_taskRunner.isRunning())
        generateInitFiles(pericentre, planetDistancesA, planetDistancesB, i,
                          numberOfOrientations);
      else
        return false;
    }
  }

  saveSimulationParameters(m_simulationParams);
  return true;
}

void InitFileGenerator::generateInitFiles(
    std::string const &pericentre,
    std::vector<std::string> const &planetDistancesA,
    std::vector<std::string> const &planetDistancesB,
    std::size_t planetDistanceIndex, std::size_t numberOfOrientations) {
  if (isThreeBodies()) {
    generate3BodyInitFiles(pericentre, planetDistancesA[planetDistanceIndex],
                           numberOfOrientations);
  } else {
    generate4BodyInitFiles(pericentre, planetDistancesA[planetDistanceIndex],
                           planetDistancesB[planetDistanceIndex],
                           numberOfOrientations);
  }
}

void InitFileGenerator::generate3BodyInitFiles(
    std::string const &pericentre, std::string const &planetDistance,
    std::size_t numberOfOrientations) {
  for (auto index = 1u; index < numberOfOrientations + 1; ++index) {
    generate3BodyInitFile(pericentre, planetDistance, index);
    m_taskRunner.reportProgress();
  }
}

void InitFileGenerator::generate3BodyInitFile(std::string const &pericentre,
                                              std::string const &planetDistance,
                                              std::size_t orientationIndex) {
  auto const phi = randomNumber(0, 360);
  auto const inclination = randomNumber(0, 360);
  generate3BodyInitFile(pericentre, planetDistance, orientationIndex, phi,
                        inclination);
}

void InitFileGenerator::generate3BodyInitFile(std::string const &pericentre,
                                              std::string const &planetDistance,
                                              std::size_t orientationIndex,
                                              std::size_t phi,
                                              std::size_t inclination) {
  generate3BodyInitFile(
      generate3BodyInitFilename(pericentre, planetDistance, orientationIndex),
      std::stod(pericentre), std::stod(planetDistance), orientationIndex, phi,
      inclination);
}

void InitFileGenerator::generate3BodyInitFile(
    std::string const &filename, double pericentre, double planetDistance,
    std::size_t orientationIndex, std::size_t phi, std::size_t inclination) {
  auto const trueAnomaly = InitHeaderData::trueAnomaly(planetDistance);
  auto const star = createStar(pericentre, trueAnomaly);
  auto const planet = createPlanet(*star, planetDistance, phi, inclination);

  std::string fileText = generateInitHeader(filename, planetDistance);
  generateFileText(fileText, *blackHole, *star, *planet);
  createFile(filename + ".init", fileText);

  addInitSimulationParams(InitSimulationParams(filename, pericentre,
                                               planetDistance, orientationIndex,
                                               phi, inclination));
}

void InitFileGenerator::generate4BodyInitFiles(
    std::string const &pericentre, std::string const &planetDistanceA,
    std::string const &planetDistanceB, std::size_t numberOfOrientations) {
  for (auto index = 1u; index < numberOfOrientations + 1; ++index) {
    generate4BodyInitFile(pericentre, planetDistanceA, planetDistanceB, index);
    m_taskRunner.reportProgress();
  }
}

void InitFileGenerator::generate4BodyInitFile(
    std::string const &pericentre, std::string const &planetDistanceA,
    std::string const &planetDistanceB, std::size_t orientationIndex) {
  auto const phi = randomNumber(0, 360);
  auto const inclination = randomNumber(0, 360);
  generate4BodyInitFile(pericentre, planetDistanceA, planetDistanceB,
                        orientationIndex, phi, inclination);
}

void InitFileGenerator::generate4BodyInitFile(
    std::string const &pericentre, std::string const &planetDistanceA,
    std::string const &planetDistanceB, std::size_t orientationIndex,
    std::size_t phi, std::size_t inclination) {
  generate4BodyInitFile(
      generate4BodyInitFilename(pericentre, planetDistanceA, planetDistanceB,
                                orientationIndex),
      std::stod(pericentre), std::stod(planetDistanceA),
      std::stod(planetDistanceB), orientationIndex, phi, inclination);
}

void InitFileGenerator::generate4BodyInitFile(
    std::string const &filename, double pericentre, double planetDistanceA,
    double planetDistanceB, std::size_t orientationIndex, std::size_t phi,
    std::size_t inclination) {
  auto const largestPlanetDistance =
      planetDistanceA > planetDistanceB ? planetDistanceA : planetDistanceB;
  auto const trueAnomaly = InitHeaderData::trueAnomaly(largestPlanetDistance);

  auto const star = createStar(pericentre, trueAnomaly);
  auto const planetA = createPlanet(*star, planetDistanceA, phi, inclination);
  auto const planetB = createPlanet(*star, planetDistanceB, phi, inclination);

  std::string fileText = generateInitHeader(filename, largestPlanetDistance);
  generateFileText(fileText, *blackHole, *star, *planetA, *planetB);
  createFile(filename + ".init", fileText);

  addInitSimulationParams(InitSimulationParams(
      filename, pericentre, planetDistanceA, planetDistanceB, orientationIndex,
      phi, inclination));
}

std::string InitFileGenerator::generate3BodyInitFilename(
    std::string const &pericentre, std::string const &planetDistance,
    std::size_t const &orientationIndex) const {
  return "p" + pericentre + "_r" + planetDistance + "_o" +
         std::to_string(orientationIndex);
}

std::string InitFileGenerator::generate4BodyInitFilename(
    std::string const &pericentre, std::string const &planetDistanceA,
    std::string const &planetDistanceB,
    std::size_t const &orientationIndex) const {
  return "p" + pericentre + "_r" + planetDistanceA + "+" + planetDistanceB +
         "_o" + std::to_string(orientationIndex);
}

std::string InitFileGenerator::generateInitHeader(std::string const &filename,
                                                  double planetDistance) const {
  auto const numBodies = std::to_string(InitHeaderData::numberOfBodies());
  auto const step = std::to_string(InitHeaderData::timeStep(planetDistance));
  auto const numTimeSteps =
      std::to_string(InitHeaderData::numberOfTimeStep(planetDistance));
  return "-1 " + numBodies + " " + step + " " + numTimeSteps +
         " 0.000000 0.000000 1.d0 1.d-3 0.d0 0 " + filename + ".out 1 1";
}

void InitFileGenerator::saveSimulationParameters(
    std::vector<InitSimulationParams> const &parameters) const {
  createFile("simulation_parameters.txt",
             generateSimulationParametersText(parameters));
}

std::string InitFileGenerator::generateSimulationParametersText(
    std::vector<InitSimulationParams> const &parameters) const {
  std::string fileText = generateSimulationParametersHeader(parameters);
  for (auto const &parameterSet : parameters)
    fileText += generateSimulationParametersLine(parameterSet);
  return std::move(fileText);
}

std::string InitFileGenerator::generateSimulationParametersHeader(
    std::vector<InitSimulationParams> const &parameters) const {
  if (OtherSimulationSettings::m_hasSinglePlanet)
    return "Index  Pericentre  PlanetDistance  Phi  Inclination";
  return "Index  Pericentre  PlanetDistanceA  PlanetDistanceB  Phi  "
         "Inclination";
}

std::string InitFileGenerator::generateSimulationParametersLine(
    InitSimulationParams const &parameters) const {
  return "\n" + std::to_string(parameters.m_orientationIndex) + " " +
         formatSimParameter(parameters.m_pericentre) + " " +
         generateSimulationPlanetDistancesSubLine(parameters) + " " +
         std::to_string(parameters.m_phi) + " " +
         std::to_string(parameters.m_inclination);
}

std::string InitFileGenerator::generateSimulationPlanetDistancesSubLine(
    InitSimulationParams const &parameters) const {
  auto const subLine = formatSimParameter(parameters.m_planetDistances[0]);
  if (OtherSimulationSettings::m_hasSinglePlanet)
    return std::move(subLine);
  return subLine + " " + formatSimParameter(parameters.m_planetDistances[1]);
}

bool InitFileGenerator::isThreeBodies() const {
  return InitHeaderData::numberOfBodies() == 3;
}
