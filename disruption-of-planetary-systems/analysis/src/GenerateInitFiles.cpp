#include "GenerateInitFiles.h"
#include "Body.h"
#include "SimParameters.h"

#include "FileManager.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <cmath>
#include <cstdlib>

static double const G = 1.0;
static Body *BLACK_HOLE = new Body(4000000.0, XYZComponents(0.0, 0.0, 0.0),
                                   XYZComponents(0.0, 0.0, 0.0));

namespace {

std::size_t randomNumber(int lower, int higher) {
  return static_cast<std::size_t>(rand() % (higher - lower) + lower);
}

} // namespace

InitFileGenerator::InitFileGenerator(std::string const &directory,
                                     double timeStep,
                                     std::size_t numberOfTimeSteps,
                                     double trueAnomaly)
    : m_fileManager(std::make_unique<FileManager>()), m_directory(directory),
      m_timeStep(timeStep), m_numberOfTimeSteps(numberOfTimeSteps),
      m_trueAnom(trueAnomaly) {}

InitFileGenerator::~InitFileGenerator() {}

void InitFileGenerator::setTimeStep(double timeStep) { m_timeStep = timeStep; }

void InitFileGenerator::setNumberOfTimeSteps(std::size_t numberOfTimeSteps) {
  m_numberOfTimeSteps = numberOfTimeSteps;
}

void InitFileGenerator::setTrueAnomaly(double trueAnomaly) {
  m_trueAnom = trueAnomaly * (M_PI / 180);
}

std::vector<SimParameters *> const &
InitFileGenerator::simulationParameters() const {
  return m_simulationParameters;
}

void InitFileGenerator::resetInitData(std::size_t numberOfInitFiles) {
  m_simulationParameters.clear();
  m_simulationParameters.reserve(numberOfInitFiles);
}

void InitFileGenerator::updateInitData(std::string const &initFilename,
                                       double pericentre, double planetDistance,
                                       std::size_t orientationIndex,
                                       std::size_t phi,
                                       std::size_t inclination) {
  m_simulationParameters.emplace_back(
      new SimParameters(initFilename, pericentre, planetDistance,
                        orientationIndex, phi, inclination));
}

void InitFileGenerator::generateInitFiles(
    std::vector<std::string> const &pericentres,
    std::vector<std::string> const &planetDistances,
    std::size_t const &numberOfOrientations) {
  resetInitData(pericentres.size() * planetDistances.size() *
                numberOfOrientations);

  for (auto const &pericentre : pericentres)
    for (auto const &planetDistance : planetDistances)
      for (auto index = 1u; index < numberOfOrientations + 1; ++index)
        generateInitFile(pericentre, planetDistance, index);

  saveParameters(simulationParameters());
}

void InitFileGenerator::generateInitFile(std::string const &pericentre,
                                         std::string const &planetDistance,
                                         std::size_t orientationIndex) {
  auto const phi = randomNumber(0, 360);
  auto const inclination = randomNumber(0, 360);
  generateInitFile(pericentre, planetDistance, orientationIndex, phi,
                   inclination);
}

void InitFileGenerator::generateInitFile(std::string const &pericentre,
                                         std::string const &planetDistance,
                                         std::size_t orientationIndex,
                                         std::size_t phi,
                                         std::size_t inclination) {
  auto const filename =
      generateInitFilename(pericentre, planetDistance, orientationIndex);
  generateInitFile(filename, pericentre, planetDistance, phi, inclination);

  updateInitData(filename, std::stod(pericentre), std::stod(planetDistance),
                 orientationIndex, phi, inclination);
}

void InitFileGenerator::generateInitFile(std::string const &initFilename,
                                         std::string const &pericentre,
                                         std::string const &planetDistance,
                                         std::size_t phi,
                                         std::size_t inclination) const {
  auto const star = generateStarData(std::stod(pericentre));
  auto const planet =
      generatePlanetData(star, std::stod(planetDistance), phi, inclination);

  generateInitFile(initFilename, BLACK_HOLE, star, planet);
}

void InitFileGenerator::generateInitFile(std::string const &initFilename,
                                         Body const *blackHole,
                                         Body const *star,
                                         Body const *planet) const {
  auto const data = generateFileText(initFilename, blackHole, star, planet);

  m_fileManager->setFilename(m_directory + initFilename + ".init");
  m_fileManager->createNewFile(data);
}

std::string InitFileGenerator::generateFileText(std::string const &initFilename,
                                                Body const *blackHole,
                                                Body const *star,
                                                Body const *planet) const {
  std::string fileText = "-1 3 " + std::to_string(m_timeStep) + " " +
                         std::to_string(m_numberOfTimeSteps) +
                         " 0.000000 0.000000 1.d0 1.d-3 0.d0 0 " +
                         initFilename + ".out 1 1";
  addBodyData(fileText, blackHole);
  addBodyData(fileText, star);
  addBodyData(fileText, planet);
  return std::move(fileText);
}

void InitFileGenerator::addBodyData(std::string &fileText,
                                    Body const *body) const {
  auto const mass = std::to_string(body->mass());
  auto const position = body->position(BodyTime::Initial);
  auto const velocity = body->velocity(BodyTime::Initial);

  fileText += "\n  " + mass + "   " + std::to_string(position.compX()) + "   " +
              std::to_string(position.compY()) + "   " +
              std::to_string(position.compZ()) + "   " +
              std::to_string(velocity.compX()) + "   " +
              std::to_string(velocity.compY()) + "   " +
              std::to_string(velocity.compZ());
}

std::string InitFileGenerator::generateInitFilename(
    std::string const &pericentre, std::string const &planetDistance,
    std::size_t const &orientationIndex) const {
  return "p" + pericentre + "_r" + planetDistance + "_o" +
         std::to_string(orientationIndex);
}

Body const *InitFileGenerator::generateStarData(double pericentre) const {
  // Calculate initial position of star
  auto const x = 2.0 * pericentre * cos(m_trueAnom) / (1 + cos(m_trueAnom));
  auto const y = 2.0 * pericentre * sin(m_trueAnom) / (1 + cos(m_trueAnom));
  auto const z = 0.0;

  // Calculate initial velocity of star
  auto const bhPosition = BLACK_HOLE->position(BodyTime::Initial);

  auto const r =
      sqrt(pow(x - bhPosition.compX(), 2) + pow(y - bhPosition.compY(), 2) +
           pow(z - bhPosition.compZ(), 2));
  auto const parabolicVelocity = sqrt(2.0 * G * BLACK_HOLE->mass() / r);

  auto const vx = parabolicVelocity * sin(m_trueAnom / 2);
  auto const vy = -parabolicVelocity * cos(m_trueAnom / 2);

  return new Body(1.0, XYZComponents(std::move(x), std::move(y), std::move(z)),
                  XYZComponents(std::move(vx), std::move(vy), 0.0));
}

Body const *
InitFileGenerator::generatePlanetData(Body const *star, double planetDistance,
                                      std::size_t phi,
                                      std::size_t inclination) const {
  // Calculate initial position of planet
  auto const starPos = star->position(BodyTime::Initial);

  auto const x = starPos.compX() + planetDistance * cos(inclination) * cos(phi);
  auto const y = starPos.compY() + planetDistance * cos(inclination) * sin(phi);
  auto const z = starPos.compZ() + planetDistance * sin(inclination);

  // Calculate initial velocity of planet
  auto const circularVelocity = sqrt(G * star->mass() / planetDistance);
  auto const starVelocity = star->velocity(BodyTime::Initial);

  auto const vx = starVelocity.compX() + circularVelocity * sin(phi);
  auto const vy = starVelocity.compY() - circularVelocity * cos(phi);

  return new Body(0.0009543,
                  XYZComponents(std::move(x), std::move(y), std::move(z)),
                  XYZComponents(std::move(vx), std::move(vy), 0.0));
}

void InitFileGenerator::saveParameters(
    std::vector<SimParameters *> const &parameters) const {
  FileManager *fileManager =
      new FileManager(m_directory + "simulation_parameters.txt");
  fileManager->createNewFile(generateFileText(parameters));
}

std::string InitFileGenerator::generateFileText(
    std::vector<SimParameters *> const &parameters) const {
  std::string fileText = "Index  Pericentre  PlanetDistance  Phi  Inclination";
  for (auto const &parameterSet : parameters)
    fileText += generateFileLine(parameterSet);
  return std::move(fileText);
}

std::string
InitFileGenerator::generateFileLine(SimParameters const *parameters) const {
  return "\n" + std::to_string(parameters->orientationIndex()) + " " +
         std::to_string(static_cast<int>(parameters->pericentre())) + ".0 " +
         std::to_string(static_cast<int>(parameters->planetDistance())) +
         ".0 " + std::to_string(parameters->phi()) + " " +
         std::to_string(parameters->inclination());
}
