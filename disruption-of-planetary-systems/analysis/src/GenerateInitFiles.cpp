#define _USE_MATH_DEFINES

#include "GenerateInitFiles.h"

#include "Body.h"
#include "FileManager.h"
#include "SimParameters.h"

#include <cmath>
#include <cstdlib>
#include <math.h>
#include <mutex>
#include <thread>

static double const G = 1.0;
static double const TRUE_ANOMALY = 164 * (M_PI / 180);
static Body *BLACK_HOLE =
    new Body(4000000.0, std::vector<double>{0.0, 0.0, 0.0},
             std::vector<double>{0.0, 0.0, 0.0});

std::mutex mutex;

namespace {

std::size_t randomNumber(int lower, int higher) {
  return static_cast<std::size_t>(rand() % (higher - lower) + lower);
}

} // namespace

InitFileGenerator::InitFileGenerator(std::string const &directory)
    : m_directory(directory) {}

InitFileGenerator::~InitFileGenerator() {}

std::vector<SimParameters *> InitFileGenerator::simulationParameters() const {
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
  mutex.lock();
  m_simulationParameters.emplace_back(
      new SimParameters(initFilename, pericentre, planetDistance,
                        orientationIndex, phi, inclination));
  mutex.unlock();
}

void InitFileGenerator::generateInitFiles(
    std::vector<std::string> const &pericentres,
    std::vector<std::string> const &planetDistances,
    std::size_t const &numberOfOrientations) {
  auto const numberOfInitFiles =
      pericentres.size() * planetDistances.size() * numberOfOrientations;
  resetInitData(numberOfInitFiles);

  std::vector<std::thread> threadVector;
  threadVector.reserve(numberOfInitFiles);

  for (auto const &pericentre : pericentres) {
    for (auto const &planetDistance : planetDistances) {
      for (auto orientationIndex = 1u;
           orientationIndex < numberOfOrientations + 1; ++orientationIndex) {
        threadVector.emplace_back([&]() {
          generateInitFile(pericentre, planetDistance, orientationIndex);
        });
      }
    }
  }

  for (auto &thread : threadVector)
    thread.join();
}

void InitFileGenerator::generateInitFile(std::string const &pericentre,
                                         std::string const &planetDistance,
                                         std::size_t orientationIndex) {
  auto const phi = randomNumber(0, 360);
  auto const inclination = randomNumber(0, 360);
  auto const initFilename =
      generateInitFilename(pericentre, planetDistance, orientationIndex);

  generateInitFile(initFilename, pericentre, planetDistance, phi, inclination);

  updateInitData(initFilename, std::stod(pericentre), std::stod(planetDistance),
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
                                         Body *blackHole, Body *star,
                                         Body *planet) const {
  FileManager *fileManager =
      new FileManager(m_directory + initFilename + ".init");
  /// Takes a long time to write to files
  fileManager->createNewFile(
      generateFileText(initFilename, blackHole, star, planet));
}

std::string InitFileGenerator::generateFileText(std::string const &initFilename,
                                                Body *blackHole, Body *star,
                                                Body *planet) const {
  std::string fileText = "-1 3 0.08 1200 0.000000 0.000000 1.d0 1.d-3 0.d0 0 " +
                         initFilename + ".out 1 1";
  addBodyData(fileText, blackHole);
  addBodyData(fileText, star);
  addBodyData(fileText, planet);
  return std::move(fileText);
}

void InitFileGenerator::addBodyData(std::string &fileText, Body *body) const {
  auto const mass = std::to_string(body->mass());
  auto const position = body->position();
  auto const velocity = body->velocity();

  fileText += "\n  " + mass + "   " + std::to_string(position[0]) + "   " +
              std::to_string(position[1]) + "   " +
              std::to_string(position[2]) + "   " +
              std::to_string(velocity[0]) + "   " +
              std::to_string(velocity[1]) + "   " + std::to_string(velocity[2]);
}

std::string InitFileGenerator::generateInitFilename(
    std::string const &pericentre, std::string const &planetDistance,
    std::size_t const &orientationIndex) const {
  return "p" + pericentre + "_r" + planetDistance + "_o" +
         std::to_string(orientationIndex);
}

Body *InitFileGenerator::generateStarData(double pericentre) const {
  // Calculate initial position of star
  auto const starPosition = std::vector<double>{
      2.0 * pericentre * cos(TRUE_ANOMALY) / (1 + cos(TRUE_ANOMALY)),
      2.0 * pericentre * sin(TRUE_ANOMALY) / (1 + cos(TRUE_ANOMALY)), 0.0};

  // Calculate initial velocity of star
  auto const bhPosition = BLACK_HOLE->position();

  auto const r = sqrt(pow(starPosition[0] - bhPosition[0], 2) +
                      pow(starPosition[1] - bhPosition[1], 2) +
                      pow(starPosition[2] - bhPosition[2], 2));
  auto const parabolicVelocity = sqrt(2.0 * G * BLACK_HOLE->mass() / r);

  auto const starVelocity =
      std::vector<double>{parabolicVelocity * sin(TRUE_ANOMALY / 2),
                          -parabolicVelocity * cos(TRUE_ANOMALY / 2), 0.0};

  return new Body(1.0, std::move(starPosition), std::move(starVelocity));
}

Body *InitFileGenerator::generatePlanetData(Body *star, double planetDistance,
                                            std::size_t phi,
                                            std::size_t inclination) const {
  // Calculate initial position of planet
  auto const starPosition = star->position();

  auto const x = starPosition[0] + planetDistance * cos(inclination) * cos(phi);
  auto const y = starPosition[1] + planetDistance * cos(inclination) * sin(phi);
  auto const z = starPosition[2] + planetDistance * sin(inclination);

  auto const planetPosition =
      std::vector<double>{std::move(x), std::move(y), std::move(z)};

  // Calculate initial velocity of planet
  auto const circularVelocity = sqrt(G * star->mass() / planetDistance);
  auto const starVelocity = star->velocity();

  auto const vx = starVelocity[0] + circularVelocity * sin(phi);
  auto const vy = starVelocity[1] - circularVelocity * cos(phi);

  auto const planetVelocity =
      std::vector<double>{std::move(vx), std::move(vy), 0.0};

  return new Body(0.0009543, std::move(planetPosition),
                  std::move(planetVelocity));
}
