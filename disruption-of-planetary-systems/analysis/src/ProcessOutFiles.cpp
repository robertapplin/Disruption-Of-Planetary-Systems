#include "ProcessOutFiles.h"

#include "Body.h"
#include "InitSimulationParams.h"
#include "SimulationConstants.h"
#include "SimulationResult.h"
#include "XYZComponents.h"

#include "FileManager.h"
#include "Logger.h"
#include "TaskRunner.h"
#include "ThreadPool.h"

#include <algorithm>

using namespace SimulationConstants;

OutFileProcessor::OutFileProcessor(std::string const &directory)
    : m_mutex(), m_directory(directory),
      m_taskRunner(TaskRunner::getInstance()) {}

OutFileProcessor::~OutFileProcessor() {}

void OutFileProcessor::resetProcessor(std::size_t numberOfOutFiles) {
  m_resultsA.clear();
  m_resultsB.clear();
  m_taskRunner.setTask("Processing out files...", 20.0, 100.0);
  m_taskRunner.setNumberOfSteps(numberOfOutFiles);
}

bool OutFileProcessor::performAnalysis(
    std::vector<InitSimulationParams> const &simulationParameters) {
  resetProcessor(simulationParameters.size());

  processOutFiles(simulationParameters);

  saveResults();
  return true;
}

void OutFileProcessor::processOutFiles(
    std::vector<InitSimulationParams> const &simulationParameters) {
  ThreadPool pool(5);

  for (auto const &parameters : simulationParameters) {
    pool.addToQueue([&]() {
      if (m_taskRunner.isRunning()) {
        processOutFile(parameters);
        m_taskRunner.reportProgress();
      }
    });
  }
}

void OutFileProcessor::processOutFile(InitSimulationParams const &parameters) {
  try {
    processOutFile(parameters, loadOutFile(parameters));
  } catch (std::runtime_error const &error) {
    m_taskRunner.stopTask();
    Logger::getInstance().addLog(LogType::Error,
                                 std::string("Processing out files failed: ") +
                                     error.what());
  }
}

void OutFileProcessor::processOutFile(
    InitSimulationParams const &parameters,
    std::vector<std::unique_ptr<Body>> const &bodies) {
  if (OtherSimulationSettings::m_hasSinglePlanet)
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
                           parameters.m_planetDistances[0], PlanetID::A);
  computeSimulationResults(parameters, blackHole, star, planetB,
                           parameters.m_planetDistances[1], PlanetID::B);
}

void OutFileProcessor::computeSimulationResults(
    InitSimulationParams const &parameters, Body const &blackHole,
    Body const &star, Body const &planet, double planetDistance,
    PlanetID const &planetID) {
  auto const stepIndex = planet.numberOfTimeSteps() - 1;

  auto const boundToStar = isBound(planet, star, stepIndex);
  bool boundToBlackHole(false);
  if (!boundToStar)
    boundToBlackHole = isBound(planet, blackHole, stepIndex);

  auto const bhOrbitProps = calculateOrbitalProperties(
      blackHole, planet, stepIndex, boundToBlackHole);
  auto const starOrbitProps =
      calculateOrbitalProperties(star, planet, stepIndex, boundToStar);

  addResult(parameters.m_pericentre, planetDistance, boundToBlackHole,
            boundToStar, bhOrbitProps.first, starOrbitProps.first,
            bhOrbitProps.second, starOrbitProps.second, planetID);
}

std::vector<std::unique_ptr<Body>>
OutFileProcessor::loadOutFile(InitSimulationParams const &parameters) const {
  std::ifstream fileStream(m_directory + parameters.m_filename + ".out");
  if (fileStream.is_open())
    return loadOutFile(fileStream);
  throw std::runtime_error("The " + parameters.m_filename +
                           ".out file does not exist.");
}

std::vector<std::unique_ptr<Body>>
OutFileProcessor::loadOutFile(std::ifstream &fileStream) const {
  if (OtherSimulationSettings::m_hasSinglePlanet)
    return loadOutFile3Body(fileStream);
  return loadOutFile4Body(fileStream);
}

std::vector<std::unique_ptr<Body>>
OutFileProcessor::loadOutFile3Body(std::ifstream &fileStream) const {
  std::vector<XYZComponents> bhPositions, bhVelocities, starPositions,
      starVelocities, planetPositions, planetVelocities;

  double a, bhMass, bhX, bhY, bhZ, bhVx, bhVy, bhVz, sMass, sX, sY, sZ, sVx,
      sVy, sVz, pMass, pX, pY, pZ, pVx, pVy, pVz;

  while (fileStream >> a >> bhMass >> bhX >> bhY >> bhZ >> bhVx >> bhVy >>
         bhVz >> sMass >> sX >> sY >> sZ >> sVx >> sVy >> sVz >> pMass >> pX >>
         pY >> pZ >> pVx >> pVy >> pVz) {
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
OutFileProcessor::loadOutFile4Body(std::ifstream &fileStream) const {
  std::vector<XYZComponents> bhPositions, bhVelocities, starPositions,
      starVelocities, planet1Positions, planet1Velocities, planet2Positions,
      planet2Velocities;

  double a, bhMass, bhX, bhY, bhZ, bhVx, bhVy, bhVz, sMass, sX, sY, sZ, sVx,
      sVy, sVz, p1Mass, p1X, p1Y, p1Z, p1Vx, p1Vy, p1Vz, p2Mass, p2X, p2Y, p2Z,
      p2Vx, p2Vy, p2Vz;

  while (fileStream >> a >> bhMass >> bhX >> bhY >> bhZ >> bhVx >> bhVy >>
         bhVz >> sMass >> sX >> sY >> sZ >> sVx >> sVy >> sVz >> p1Mass >>
         p1X >> p1Y >> p1Z >> p1Vx >> p1Vy >> p1Vz >> p2Mass >> p2X >> p2Y >>
         p2Z >> p2Vx >> p2Vy >> p2Vz) {
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
    auto const semiMajorAxis = calculateSemiMajorAxis(body1, body2, index);
    auto const eccentricity =
        calculateEccentricity(body1, body2, semiMajorAxis, index);
    return std::pair<double, double>(std::move(semiMajorAxis),
                                     std::move(eccentricity));
  }
  return std::pair<double, double>(0.0, 0.0);
}

double OutFileProcessor::calculateSemiMajorAxis(Body const &body1,
                                                Body const &body2,
                                                std::size_t index) const {
  auto const m_total = body1.mass() + body2.mass();
  auto const r = body1.relativePositionMagnitude(body2, index);
  auto const v = body1.relativeVelocityMagnitude(body2, index);
  return pow(2.0 / r - (pow(v, 2) / (G * m_total)), -1);
}

double OutFileProcessor::calculateEccentricity(Body const &body1,
                                               Body const &body2,
                                               double semiMajorAxis,
                                               std::size_t index) const {
  auto const m_total = body1.mass() + body2.mass();
  auto const relativePosition = body1.position(index) - body2.position(index);
  auto const relativeVelocity = body1.velocity(index) - body2.velocity(index);
  auto const h = relativePosition.crossProduct(relativeVelocity).magnitude();

  return sqrt(1.0 - pow(h, 2) / (G * m_total * semiMajorAxis));
}

void OutFileProcessor::addResult(double pericentre, double planetDistance,
                                 bool bhBound, bool starBound,
                                 double semiMajorBh, double semiMajorStar,
                                 double eccentricityBh, double eccentricityStar,
                                 PlanetID const &planetID) {
  if (planetID == PlanetID::None || planetID == PlanetID::A) {
    addResult(m_resultsA, pericentre, planetDistance, bhBound, starBound,
              semiMajorBh, semiMajorStar, eccentricityBh, eccentricityStar);
  } else if (planetID == PlanetID::B) {
    addResult(m_resultsB, pericentre, planetDistance, bhBound, starBound,
              semiMajorBh, semiMajorStar, eccentricityBh, eccentricityStar);
  }
}

void OutFileProcessor::addResult(
    std::map<std::pair<double, double>, MutableResult> &results,
    double pericentre, double planetDistance, bool bhBound, bool starBound,
    double semiMajorBh, double semiMajorStar, double eccentricityBh,
    double eccentricityStar) {
  if (!updateResult(results, pericentre, planetDistance, bhBound, starBound,
                    semiMajorBh, semiMajorStar, eccentricityBh,
                    eccentricityStar)) {
    std::unique_lock<std::mutex> lock(m_mutex);
    results[std::make_pair(pericentre, planetDistance)] = MutableResult(
        calculateHillsRadius(pericentre), bhBound, starBound, semiMajorBh,
        semiMajorStar, eccentricityBh, eccentricityStar);
  }
}

bool OutFileProcessor::updateResult(
    std::map<std::pair<double, double>, MutableResult> &results,
    double pericentre, double planetDistance, bool bhBound, bool starBound,
    double semiMajorBh, double semiMajorStar, double eccentricityBh,
    double eccentricityStar) {
  auto const predicate = [&pericentre, &planetDistance](auto const &result) {
    return pericentre == result.first.first &&
           planetDistance == result.first.second;
  };
  return updateResult(results, pericentre, planetDistance, bhBound, starBound,
                      semiMajorBh, semiMajorStar, eccentricityBh,
                      eccentricityStar, predicate);
}

template <typename Predicate>
bool OutFileProcessor::updateResult(
    std::map<std::pair<double, double>, MutableResult> &results,
    double pericentre, double planetDistance, bool bhBound, bool starBound,
    double semiMajorBh, double semiMajorStar, double eccentricityBh,
    double eccentricityStar, Predicate const &predicate) {
  std::unique_lock<std::mutex> lock(m_mutex);

  auto const iter = std::find_if(results.begin(), results.end(), predicate);
  if (iter != results.end()) {
    results[std::make_pair(pericentre, planetDistance)] =
        iter->second + MutableResult(calculateHillsRadius(pericentre), bhBound,
                                     starBound, semiMajorBh, semiMajorStar,
                                     eccentricityBh, eccentricityStar);
    return true;
  }
  return false;
}

void OutFileProcessor::saveResults() const {
  if (OtherSimulationSettings::m_hasSinglePlanet)
    save3BodyResults();
  else
    save4BodyResults();
}

void OutFileProcessor::save3BodyResults() const {
  saveResults("simulation_results.txt", generateResultsFileText(m_resultsA));
}

void OutFileProcessor::save4BodyResults() const {
  saveResults("simulation_resultsA.txt", generateResultsFileText(m_resultsA));
  saveResults("simulation_resultsB.txt", generateResultsFileText(m_resultsB));

  if (OtherSimulationSettings::m_combinePlanetResults)
    saveResults("simulation_results.txt",
                generateResultsFileText(combineResults()));
}

void OutFileProcessor::saveResults(std::string const &filename,
                                   std::string const &fileText) const {
  auto fileManager = std::make_unique<FileManager>(m_directory + filename);
  auto const header =
      "Pericentre  PlanetDistance  HillsRadius  BhBoundFraction  "
      "StarBoundFraction  UnboundFraction  BhBoundFractionError  "
      "StarBoundFractionError  UnboundFractionError  SemiMajorBh  "
      "SemiMajorStar  EccentricityBh  EccentricityStar";
  fileManager->createNewFile(header + fileText);
}

std::map<std::pair<double, double>, MutableResult>
OutFileProcessor::combineResults() const {
  auto combinedResults = m_resultsA;
  combinedResults.insert(m_resultsB.begin(), m_resultsB.end());
  return combinedResults;
}

std::string OutFileProcessor::generateResultsFileText(
    std::map<std::pair<double, double>, MutableResult> const &results) const {
  std::string fileText;
  for (auto const &result : results)
    fileText += generateResultFileLine(result.first, result.second);
  return std::move(fileText);
}

std::string OutFileProcessor::generateResultFileLine(
    std::pair<double, double> const &parameters,
    MutableResult const &result) const {
  auto line = "\n" + std::to_string(parameters.first) + " " +
              std::to_string(parameters.second) + " " +
              std::to_string(result.hillsRadius()) + " " +
              std::to_string(result.bhBoundFraction()) + " " +
              std::to_string(result.starBoundFraction()) + " " +
              std::to_string(result.unboundFraction()) + " " +
              std::to_string(result.bhBoundFractionError()) + " " +
              std::to_string(result.starBoundFractionError()) + " " +
              std::to_string(result.unboundFractionError());

  for (auto const &semiMajor : result.semiMajorsBh())
    line += " " + std::to_string(semiMajor);
  for (auto const &eccentricity : result.eccentricitiesBh())
    line += " " + std::to_string(eccentricity);

  return line;
}
