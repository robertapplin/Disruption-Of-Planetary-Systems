#include "ProcessOutFiles.h"

#include "Body.h"
#include "InitSimulationParams.h"
#include "SimulationConstants.h"
#include "SimulationResult.h"

#include "FileManager.h"
#include "TaskRunner.h"
#include "ThreadPool.h"

#include <algorithm>
#include <fstream>

#include <mutex>

using namespace SimulationConstants;

OutFileProcessor::OutFileProcessor(std::string const &directory)
    : m_mutex(), m_directory(directory),
      m_taskRunner(TaskRunner::getInstance()) {}

OutFileProcessor::~OutFileProcessor() {}

void OutFileProcessor::resetProcessor(std::size_t numberOfOutFiles) {
  m_results.clear();
  m_taskRunner.setTask("Processing out files...", 20.0, 100.0);
  m_taskRunner.setNumberOfSteps(numberOfOutFiles);
}

bool OutFileProcessor::processOutFiles(
    std::vector<InitSimulationParams> const &simulationParameters) {
  resetProcessor(simulationParameters.size());
  initializeResults(simulationParameters);

  ThreadPool pool(5);

  for (auto const &parameters : simulationParameters) {
    pool.addToQueue([&]() {
      if (m_taskRunner.isRunning()) {
        processOutFile(parameters);
        {
          std::unique_lock<std::mutex> lock(m_mutex);
          m_taskRunner.reportProgress();
        }
      }
    });
  }

  saveResults();
  return true;
}

void OutFileProcessor::processOutFile(InitSimulationParams const &parameters) {
  auto const bodies = loadOutFile(parameters);
  if (parameters.hasSinglePlanet())
    computeSimulationResults(parameters, *bodies[0], *bodies[1], *bodies[2],
                             parameters.m_planetDistances[0]);
  else
    computeSimulationResults4Body(parameters, *bodies[0], *bodies[1],
                                  *bodies[2], *bodies[3]);
}

void OutFileProcessor::computeSimulationResults4Body(
    InitSimulationParams const &parameters, Body const &blackHole,
    Body const &star, Body const &planetA, Body const &planetB) {
  computeSimulationResults(parameters, blackHole, star, planetA,
                           parameters.m_planetDistances[0]);
  computeSimulationResults(parameters, blackHole, star, planetB,
                           parameters.m_planetDistances[1]);
}

void OutFileProcessor::computeSimulationResults(
    InitSimulationParams const &parameters, Body const &blackHole,
    Body const &star, Body const &planet, double planetDistance) {
  auto const stepIndex = planet.numberOfTimeSteps() - 1;

  auto const boundToBlackHole = isBound(planet, blackHole, stepIndex);
  auto const boundToStar = isBound(planet, star, stepIndex);

  auto const bhOrbitProps = calculateOrbitalProperties(
      blackHole, planet, stepIndex, boundToBlackHole);
  auto const starOrbitProps =
      calculateOrbitalProperties(star, planet, stepIndex, boundToStar);

  addResult(parameters.m_pericentre, planetDistance, boundToBlackHole,
            boundToStar, bhOrbitProps.first, starOrbitProps.first,
            bhOrbitProps.second, starOrbitProps.second);
}

std::vector<std::unique_ptr<Body>>
OutFileProcessor::loadOutFile(InitSimulationParams const &parameters) const {
  if (parameters.hasSinglePlanet())
    return loadOutFile3Body(parameters.m_filename);
  return loadOutFile4Body(parameters.m_filename);
}

std::vector<std::unique_ptr<Body>>
OutFileProcessor::loadOutFile3Body(std::string const &filename) const {
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

  std::vector<std::unique_ptr<Body>> allBodies;
  allBodies.reserve(3);
  allBodies.emplace_back(std::make_unique<Body>(BH_MASS, std::move(bhPositions),
                                                std::move(bhVelocities)));
  allBodies.emplace_back(std::make_unique<Body>(
      STAR_MASS, std::move(starPositions), std::move(starVelocities)));
  allBodies.emplace_back(std::make_unique<Body>(
      PLANET_MASS, std::move(planetPositions), std::move(planetVelocities)));
  return std::move(allBodies);
}

std::vector<std::unique_ptr<Body>>
OutFileProcessor::loadOutFile4Body(std::string const &filename) const {
  std::ifstream file(m_directory + filename + ".out");

  double a, bhMass, bhX, bhY, bhZ, bhVx, bhVy, bhVz, sMass, sX, sY, sZ, sVx,
      sVy, sVz, pMass, p1X, p1Y, p1Z, p1Vx, p1Vy, p1Vz, p2X, p2Y, p2Z, p2Vx,
      p2Vy, p2Vz;

  std::vector<XYZComponents> bhPositions, bhVelocities, starPositions,
      starVelocities, planet1Positions, planet1Velocities, planet2Positions,
      planet2Velocities;

  while (file >> a >> bhMass >> bhX >> bhY >> bhZ >> bhVx >> bhVy >> bhVz >>
         sMass >> sX >> sY >> sZ >> sVx >> sVy >> sVz >> pMass >> p1X >> p1Y >>
         p1Z >> p1Vx >> p1Vy >> p1Vz >> p2X >> p2Y >> p2Z >> p2Vx >> p2Vy >>
         p2Vz) {
    bhPositions.emplace_back(XYZComponents(bhX, bhY, bhZ));
    bhVelocities.emplace_back(XYZComponents(bhVx, bhVy, bhVz));
    starPositions.emplace_back(XYZComponents(sX, sY, sZ));
    starVelocities.emplace_back(XYZComponents(sVx, sVy, sVz));
    planet1Positions.emplace_back(XYZComponents(p1X, p1Y, p1Z));
    planet1Velocities.emplace_back(XYZComponents(p1Vx, p1Vy, p1Vz));
    planet2Positions.emplace_back(XYZComponents(p2X, p2Y, p2Z));
    planet2Velocities.emplace_back(XYZComponents(p2Vx, p2Vy, p2Vz));
  }

  std::vector<std::unique_ptr<Body>> allBodies;
  allBodies.reserve(4);
  allBodies.emplace_back(std::make_unique<Body>(BH_MASS, std::move(bhPositions),
                                                std::move(bhVelocities)));
  allBodies.emplace_back(std::make_unique<Body>(
      STAR_MASS, std::move(starPositions), std::move(starVelocities)));
  allBodies.emplace_back(std::make_unique<Body>(
      PLANET_MASS, std::move(planet1Positions), std::move(planet1Velocities)));
  allBodies.emplace_back(std::make_unique<Body>(
      PLANET_MASS, std::move(planet2Positions), std::move(planet2Velocities)));
  return std::move(allBodies);
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
  return 0.5 * m * pow(v, 2) - G * otherBody.mass() * m / r;
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

double OutFileProcessor::calculateHillsRadius(double pericentre) const {
  return pericentre * pow(STAR_MASS / (3 * BH_MASS), 1.0 / 3.0);
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
  return pow(2.0 / r - (pow(v, 2) / (G * m)), -1);
}

double OutFileProcessor::calculateEccentricity(Body const &body1,
                                               Body const &body2,
                                               double semiMajorAxis,
                                               std::size_t index) const {
  auto const relativePosition = body1.position(index) - body2.position(index);
  auto const relativeVelocity = body2.velocity(index) - body2.velocity(index);
  auto const h = relativePosition.crossProduct(relativeVelocity).magnitude();

  return sqrt(1.0 - pow(h, 2) / (G * body1.mass() * semiMajorAxis));
}

void OutFileProcessor::saveResults() const {
  auto fileManager =
      std::make_unique<FileManager>(m_directory + "simulation_results.txt");
  fileManager->createNewFile(generateResultsFileText());
}

std::string OutFileProcessor::generateResultsFileText() const {
  std::string fileText =
      "Pericentre  PlanetDistance  HillsRadius  BhBoundFraction  "
      "StarBoundFraction  UnboundFraction  BhBoundFractionError  "
      "StarBoundFractionError  UnboundFractionError  SemiMajorBh  "
      "SemiMajorStar  EccentricityBh  EccentricityStar";
  for (auto const &result : m_results)
    fileText += generateResultFileLine(*result);
  return std::move(fileText);
}

std::string
OutFileProcessor::generateResultFileLine(SimulationResult const &result) const {
  return "\n" + std::to_string(result.pericentre()) + " " +
         std::to_string(result.planetDistance()) + " " +
         std::to_string(result.hillsRadius()) + " " +
         std::to_string(result.bhBoundFraction()) + " " +
         std::to_string(result.starBoundFraction()) + " " +
         std::to_string(result.unboundFraction()) + " " +
         std::to_string(result.bhBoundFractionError()) + " " +
         std::to_string(result.starBoundFractionError()) + " " +
         std::to_string(result.unboundFractionError()) + " " +
         std::to_string(result.semiMajorBh()) + " " +
         std::to_string(result.semiMajorStar()) + " " +
         std::to_string(result.eccentricityBh()) + " " +
         std::to_string(result.eccentricityStar());
}

void OutFileProcessor::initializeResults(
    std::vector<InitSimulationParams> const &simulationParameters) {
  for (auto const &parameters : simulationParameters) {
    auto const pericentre = parameters.m_pericentre;
    initializeResult(pericentre, parameters.m_planetDistances[0]);
    if (!parameters.hasSinglePlanet())
      initializeResult(pericentre, parameters.m_planetDistances[1]);
  }
}

void OutFileProcessor::initializeResult(double pericentre,
                                        double planetDistance) {
  if (!haveResult(pericentre, planetDistance))
    m_results.emplace_back(std::make_unique<SimulationResult>(
        pericentre, planetDistance, calculateHillsRadius(pericentre)));
}

bool OutFileProcessor::haveResult(double pericentre,
                                  double planetDistance) const {
  for (auto const &result : m_results)
    if (result->sameParameters(pericentre, planetDistance))
      return true;
  return false;
}

void OutFileProcessor::addResult(double pericentre, double planetDistance,
                                 bool bhBound, bool starBound,
                                 double semiMajorBh, double semiMajorStar,
                                 double eccentricityBh,
                                 double eccentricityStar) {
  std::unique_lock<std::mutex> lock(m_mutex);
  for (auto const &result : m_results) {
    if (result->sameParameters(pericentre, planetDistance)) {
      result->updateCounts(bhBound, starBound);
      result->updateAverages(semiMajorBh, semiMajorStar, eccentricityBh,
                             eccentricityStar);
      return;
    }
  }
}
