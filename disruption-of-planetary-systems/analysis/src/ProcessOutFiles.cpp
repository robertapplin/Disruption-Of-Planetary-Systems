#include "ProcessOutFiles.h"
#include "Body.h"
#include "SimParameters.h"
#include "SimResults.h";

#include "FileManager.h"
#include "Logger.h"

#include <algorithm>
#include <fstream>

#include <mutex>
#include <thread>

std::mutex mutex;

OutFileProcessor::OutFileProcessor(std::string const &directory)
    : m_directory(directory) {}

OutFileProcessor::~OutFileProcessor() {}

void OutFileProcessor::processOutFiles(
    std::vector<SimParameters *> const &simulationParameters) const {
  SimResults results;

  std::vector<std::thread> threadVector;
  threadVector.reserve(simulationParameters.size());

  for (auto const &parameters : simulationParameters)
    threadVector.emplace_back([&]() { processOutFile(*parameters, results); });

  for (auto &thread : threadVector)
    thread.join();

  saveResults(results);
}

void OutFileProcessor::processOutFile(SimParameters const &parameters,
                                      SimResults &results) const {

  auto const outData = loadOutFile(parameters.filename());
  auto const blackHole = outData[0];
  auto const star = outData[1];
  auto const planet = outData[2];

  computeSimResults(parameters, results, *blackHole, *star, *planet,
                    planet->numberOfTimeSteps() - 1);
}

void OutFileProcessor::computeSimResults(SimParameters const &parameters,
                                         SimResults &results,
                                         Body const &blackHole,
                                         Body const &star, Body const &planet,
                                         std::size_t stepIndex) const {
  auto const boundToBlackHole = isBound(planet, blackHole, stepIndex);
  auto const boundToStar = isBound(planet, star, stepIndex);

  auto const bhOrbitProps = calculateOrbitalProperties(
      blackHole, planet, stepIndex, boundToBlackHole);
  auto const starOrbitProps =
      calculateOrbitalProperties(star, planet, stepIndex, boundToStar);

  mutex.lock();
  results.addResult(parameters.pericentre(), parameters.planetDistance(),
                    boundToBlackHole, boundToStar, bhOrbitProps.first,
                    starOrbitProps.first, bhOrbitProps.second,
                    starOrbitProps.second);
  mutex.unlock();
}

std::vector<Body *>
OutFileProcessor::loadOutFile(std::string const &filename) const {
  std::ifstream file(m_directory + filename + ".out");

  double a, bhMass, bhX, bhY, bhZ, bhVx, bhVy, bhVz, sMass, sX, sY, sZ, sVx,
      sVy, sVz, pMass, pX, pY, pZ, pVx, pVy, pVz;

  std::vector<XYZComponents> bhPositions, bhVelocities, starPositions,
      starVelocities, planetPositions, planetVelocities;

  while (file >> a >> bhMass >> bhX >> bhY >> bhZ >> bhVx >> bhVy >> bhVz >>
         sMass >> sX >> sY >> sZ >> sVx >> sVy >> sVz >> pMass >> pX >> pY >>
         pZ >> pVx >> pVy >> pVz) {
    bhPositions.emplace_back(XYZComponents(bhX, bhY, bhZ));
    bhVelocities.emplace_back(XYZComponents(bhVx, bhVy, bhVz));
    starPositions.emplace_back(XYZComponents(sX, sY, sZ));
    starVelocities.emplace_back(XYZComponents(sVx, sVy, sVz));
    planetPositions.emplace_back(XYZComponents(pX, pY, pZ));
    planetVelocities.emplace_back(XYZComponents(pVx, pVy, pVz));
  }

  return std::vector<Body *>{
      new Body(4000000.0, std::move(bhPositions), std::move(bhVelocities)),
      new Body(1.0, std::move(starPositions), std::move(starVelocities)),
      new Body(0.0009543, std::move(planetPositions),
               std::move(planetVelocities))};
}

std::vector<double> OutFileProcessor::calculateBodyTotalEnergies(
    Body const &targetBody, Body const &otherBody,
    std::size_t numberOfTimeSteps) const {
  std::vector<double> totalEnergies;
  totalEnergies.reserve(numberOfTimeSteps);

  for (auto i = 0u; i < numberOfTimeSteps; ++i)
    totalEnergies.emplace_back(calculateTotalEnergy(targetBody, otherBody, i));
  return std::move(totalEnergies);
}

double OutFileProcessor::calculateTotalEnergy(Body const &targetBody,
                                              Body const &otherBody,
                                              std::size_t index) const {
  auto const m = targetBody.mass();
  auto const r = targetBody.relativePositionMagnitude(otherBody, index);
  auto const v = targetBody.relativeVelocityMagnitude(otherBody, index);
  return 0.5 * m * pow(v, 2) - otherBody.mass() * m / r;
}

bool OutFileProcessor::isBound(Body const &targetBody, Body const &otherBody,
                               std::size_t index) const {
  auto const totalEnergies =
      calculateBodyTotalEnergies(targetBody, otherBody, index);
  return isBound(totalEnergies);
}

bool OutFileProcessor::isBound(std::vector<double> const &energies) const {
  auto const maxIter = std::max_element(energies.begin(), energies.end());
  auto const minIter = std::min_element(maxIter, energies.end());

  return std::all_of(minIter, energies.end(), [](double x) { return x < 0.0; });
}

std::pair<double, double> OutFileProcessor::calculateOrbitalProperties(
    Body const &body1, Body const &body2, std::size_t index, bool bound) const {
  if (bound) {
    auto const semiMajorAxis = calculateSemiMajorAxis(body1, body2, index - 1);
    auto const eccentricity =
        calculateEccentricity(body1, body2, semiMajorAxis, index - 1);
    return std::pair<double, double>(std::move(semiMajorAxis),
                                     std::move(eccentricity));
  }
  return std::pair<double, double>(0.0, 0.0);
}

double OutFileProcessor::calculateSemiMajorAxis(Body const &body1,
                                                Body const &body2,
                                                std::size_t index) const {
  auto const m = body1.mass();
  auto const r = body1.relativePositionMagnitude(body2, index);
  auto const v = body1.relativeVelocityMagnitude(body2, index);
  return pow(2.0 / r - pow(v, 2) / m, -1);
}

double OutFileProcessor::calculateEccentricity(Body const &body1,
                                               Body const &body2,
                                               double semiMajorAxis,
                                               std::size_t index) const {
  auto const relativePosition = body1.position(index) - body2.position(index);
  auto const relativeVelocity = body2.velocity(index) - body2.velocity(index);
  auto const h = relativePosition.crossProduct(relativeVelocity).magnitude();

  return sqrt(1.0 - pow(h, 2) / (body1.mass() * semiMajorAxis));
}

void OutFileProcessor::saveResults(SimResults const &results) const {
  FileManager *fileManager =
      new FileManager(m_directory + "simulation_results.txt");
  fileManager->createNewFile(generateFileText(results));
}

std::string
OutFileProcessor::generateFileText(SimResults const &results) const {
  std::string fileText =
      "Pericentre  PlanetDistance  BhBoundFraction  "
      "StarBoundFraction  UnboundFraction  BhBoundFractionError  "
      "StarBoundFractionError  UnboundFractionError  SemiMajorBh  "
      "SemiMajorStar  EccentricityBh  EccentricityStar";
  for (auto const &result : results.simulationResults())
    fileText += generateFileLine(result);
  return std::move(fileText);
}

std::string OutFileProcessor::generateFileLine(SimResult const *result) const {
  return "\n" + std::to_string(result->pericentre()) + " " +
         std::to_string(result->planetDistance()) + " " +
         std::to_string(result->bhBoundFraction()) + " " +
         std::to_string(result->starBoundFraction()) + " " +
         std::to_string(result->unboundFraction()) + " " +
         std::to_string(result->bhBoundFractionError()) + " " +
         std::to_string(result->starBoundFractionError()) + " " +
         std::to_string(result->unboundFractionError()) + " " +
         std::to_string(result->semiMajorBh()) + " " +
         std::to_string(result->semiMajorStar()) + " " +
         std::to_string(result->eccentricityBh()) + " " +
         std::to_string(result->eccentricityStar());
}
