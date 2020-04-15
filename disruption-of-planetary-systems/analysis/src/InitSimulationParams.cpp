#include "InitSimulationParams.h"

#include <algorithm>

#define _USE_MATH_DEFINES
#include <math.h>

/*
The parameters found in the headers of .init files
*/
InitHeaderParams::InitHeaderParams() {}

InitHeaderParams::InitHeaderParams(double timeStep,
                                   std::size_t numberOfTimeSteps,
                                   double trueAnomaly)
    : m_numberOfBodies(0), m_timeStep(timeStep),
      m_numberOfTimeSteps(numberOfTimeSteps),
      m_trueAnomaly(trueAnomaly * (M_PI / 180)) {}

InitHeaderParams::~InitHeaderParams() {}

/*
The default and fixed header parameter data for .init files
*/
InitHeaderData::~InitHeaderData() {}

std::size_t InitHeaderData::numberOfBodies() {
  return m_fixedHeaderParams->m_numberOfBodies;
}

double InitHeaderData::timeStep(double planetDistance) {
  if (m_useDefaults)
    return getDefaultHeaderParams(planetDistance).m_timeStep;
  return m_fixedHeaderParams->m_timeStep;
}

std::size_t InitHeaderData::numberOfTimeStep(double planetDistance) {
  if (m_useDefaults)
    return getDefaultHeaderParams(planetDistance).m_numberOfTimeSteps;
  return m_fixedHeaderParams->m_numberOfTimeSteps;
}

double InitHeaderData::trueAnomaly(double planetDistance) {
  if (m_useDefaults)
    return getDefaultHeaderParams(planetDistance).m_trueAnomaly;
  return m_fixedHeaderParams->m_trueAnomaly;
}

InitHeaderParams const &
InitHeaderData::getDefaultHeaderParams(double planetDistance) {
  auto const predicate = [&planetDistance](auto const &pair) {
    return planetDistance < pair.first;
  };
  return std::find_if(m_defaultHeaderParams.begin(),
                      m_defaultHeaderParams.end(), predicate)
      ->second;
}

bool InitHeaderData::m_useDefaults = false;

std::unique_ptr<InitHeaderParams> InitHeaderData::m_fixedHeaderParams =
    std::make_unique<InitHeaderParams>();

std::map<double, InitHeaderParams> InitHeaderData::m_defaultHeaderParams =
    std::map<double, InitHeaderParams>{
        {5.0, InitHeaderParams(0.1, 800, 160.0)},
        {10.0, InitHeaderParams(0.1, 1800, 166.0)},
        {20.0, InitHeaderParams(0.1, 2500, 168.0)},
        {30.0, InitHeaderParams(0.1, 4000, 170.0)},
        {40.0, InitHeaderParams(0.1, 6500, 172.0)},
        {50.0, InitHeaderParams(0.1, 14000, 173.0)},
        {60.0, InitHeaderParams(0.1, 18000, 174.0)},
        {100.0, InitHeaderParams(0.1, 24000, 175.0)}};
;

/*
The simulation parameters used to calculate body positions and velocities
*/
InitSimulationParams::InitSimulationParams(
    std::string const &filename, double pericentre, double planetDistance,
    std::size_t orientationIndex, std::size_t phi, std::size_t inclination)
    : m_filename(filename), m_pericentre(pericentre),
      m_orientationIndex(orientationIndex), m_phi(phi),
      m_inclination(inclination) {
  m_planetDistances.emplace_back(planetDistance);
}

InitSimulationParams::InitSimulationParams(
    std::string const &filename, double pericentre, double planetDistanceA,
    double planetDistanceB, std::size_t orientationIndex, std::size_t phi,
    std::size_t inclination)
    : m_filename(filename), m_pericentre(pericentre),
      m_orientationIndex(orientationIndex), m_phi(phi),
      m_inclination(inclination) {
  m_planetDistances.emplace_back(planetDistanceA);
  m_planetDistances.emplace_back(planetDistanceB);
}

InitSimulationParams::~InitSimulationParams() {}

bool InitSimulationParams::hasSinglePlanet() const {
  return m_planetDistances.size() == 1;
}

bool InitSimulationParams::operator!=(
    InitSimulationParams const &otherParams) const {
  return m_filename != otherParams.m_filename;
}
