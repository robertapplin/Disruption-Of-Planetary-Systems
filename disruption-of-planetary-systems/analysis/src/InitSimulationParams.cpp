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

double InitHeaderData::timeStep(double pericentre, double planetDistance) {
  if (OtherSimulationSettings::m_useDefaults)
    return getDefaultHeaderParams(pericentre, planetDistance).m_timeStep;
  return m_fixedHeaderParams->m_timeStep;
}

std::size_t InitHeaderData::numberOfTimeStep(double pericentre,
                                             double planetDistance) {
  if (OtherSimulationSettings::m_useDefaults)
    return getDefaultHeaderParams(pericentre, planetDistance)
        .m_numberOfTimeSteps;
  return m_fixedHeaderParams->m_numberOfTimeSteps;
}

double InitHeaderData::trueAnomaly(double pericentre, double planetDistance) {
  if (OtherSimulationSettings::m_useDefaults)
    return getDefaultHeaderParams(pericentre, planetDistance).m_trueAnomaly;
  return m_fixedHeaderParams->m_trueAnomaly;
}

InitHeaderParams const &
InitHeaderData::getDefaultHeaderParams(double pericentre,
                                       double planetDistance) {
  auto const predicate = [&pericentre, &planetDistance](auto const &pair) {
    return pericentre <= pair.first.first &&
           planetDistance <= pair.first.second;
  };
  return std::find_if(m_defaultHeaderParams.begin(),
                      m_defaultHeaderParams.end(), predicate)
      ->second;
}

std::unique_ptr<InitHeaderParams> InitHeaderData::m_fixedHeaderParams =
    std::make_unique<InitHeaderParams>();

std::map<std::pair<double, double>, InitHeaderParams>
    InitHeaderData::m_defaultHeaderParams =
        std::map<std::pair<double, double>, InitHeaderParams>{
            {std::make_pair(100.0, 5.0), InitHeaderParams(0.1, 3000, 170.0)},
            {std::make_pair(100.0, 10.0), InitHeaderParams(0.1, 3500, 172.0)},
            {std::make_pair(100.0, 20.0), InitHeaderParams(0.1, 12000, 174.5)},
            {std::make_pair(100.0, 30.0), InitHeaderParams(0.1, 24000, 175.5)},
            {std::make_pair(100.0, 40.0), InitHeaderParams(0.1, 30000, 176.5)},
            {std::make_pair(100.0, 50.0), InitHeaderParams(0.1, 32000, 176.8)},
            {std::make_pair(100.0, 60.0), InitHeaderParams(0.1, 35000, 177.0)},
            {std::make_pair(200.0, 5.0), InitHeaderParams(0.1, 2000, 168.0)},
            {std::make_pair(200.0, 10.0), InitHeaderParams(0.1, 3000, 171.0)},
            {std::make_pair(200.0, 20.0), InitHeaderParams(0.1, 8000, 173.5)},
            {std::make_pair(200.0, 30.0), InitHeaderParams(0.1, 13000, 174.5)},
            {std::make_pair(200.0, 40.0), InitHeaderParams(0.1, 20000, 175.2)},
            {std::make_pair(200.0, 50.0), InitHeaderParams(0.1, 28000, 176.0)},
            {std::make_pair(200.0, 60.0), InitHeaderParams(0.1, 36000, 176.2)},
            {std::make_pair(300.0, 5.0), InitHeaderParams(0.1, 800, 160.0)},
            {std::make_pair(300.0, 10.0), InitHeaderParams(0.1, 2000, 167.0)},
            {std::make_pair(300.0, 20.0), InitHeaderParams(0.1, 6000, 169.5)},
            {std::make_pair(300.0, 30.0), InitHeaderParams(0.1, 8000, 171.5)},
            {std::make_pair(300.0, 40.0), InitHeaderParams(0.1, 11000, 172.0)},
            {std::make_pair(300.0, 50.0), InitHeaderParams(0.1, 14000, 173.5)},
            {std::make_pair(300.0, 60.0), InitHeaderParams(0.1, 18000, 174.0)},
            {std::make_pair(400.0, 5.0), InitHeaderParams(0.1, 800, 160.0)},
            {std::make_pair(400.0, 10.0), InitHeaderParams(0.1, 2000, 166.0)},
            {std::make_pair(400.0, 20.0), InitHeaderParams(0.1, 6000, 169.5)},
            {std::make_pair(400.0, 30.0), InitHeaderParams(0.1, 8000, 171.5)},
            {std::make_pair(400.0, 40.0), InitHeaderParams(0.1, 11000, 172.0)},
            {std::make_pair(400.0, 50.0), InitHeaderParams(0.1, 15000, 173.5)},
            {std::make_pair(400.0, 60.0), InitHeaderParams(0.1, 24000, 174.0)},
            {std::make_pair(500.0, 5.0), InitHeaderParams(0.1, 800, 160.0)},
            {std::make_pair(500.0, 10.0), InitHeaderParams(0.1, 2800, 165.0)},
            {std::make_pair(500.0, 20.0), InitHeaderParams(0.1, 6000, 169.5)},
            {std::make_pair(500.0, 30.0), InitHeaderParams(0.1, 12000, 171.5)},
            {std::make_pair(500.0, 40.0), InitHeaderParams(0.1, 20000, 172.0)},
            {std::make_pair(500.0, 50.0), InitHeaderParams(0.1, 30000, 173.5)},
            {std::make_pair(500.0, 60.0), InitHeaderParams(0.1, 37000, 174.0)},
            {std::make_pair(600.0, 5.0), InitHeaderParams(0.1, 1000, 158.0)},
            {std::make_pair(600.0, 10.0), InitHeaderParams(0.1, 2800, 166.0)},
            {std::make_pair(600.0, 20.0), InitHeaderParams(0.1, 6000, 168.0)},
            {std::make_pair(600.0, 30.0), InitHeaderParams(0.1, 12000, 170.0)},
            {std::make_pair(600.0, 40.0), InitHeaderParams(0.1, 20000, 172.0)},
            {std::make_pair(600.0, 50.0), InitHeaderParams(0.1, 30000, 173.5)},
            {std::make_pair(600.0, 60.0), InitHeaderParams(0.1, 37000, 174.0)}};

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

bool InitSimulationParams::operator!=(
    InitSimulationParams const &otherParams) const {
  return m_filename != otherParams.m_filename;
}

/*
Other settings used for the simulation
*/
bool OtherSimulationSettings::m_hasSinglePlanet = true;

bool OtherSimulationSettings::m_combinePlanetResults = true;

bool OtherSimulationSettings::m_useDefaults = true;
