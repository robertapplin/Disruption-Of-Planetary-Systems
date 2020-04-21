#ifndef INITSIMULATIONPARAMS_H
#define INITSIMULATIONPARAMS_H

#include <map>
#include <memory>
#include <string>
#include <vector>

/*
The parameters found in the headers of .init files
*/
struct InitHeaderParams {
  InitHeaderParams();
  InitHeaderParams(double timeStep, std::size_t numberOfTimeSteps,
                   double trueAnomaly);
  ~InitHeaderParams();

  std::size_t m_numberOfBodies;
  double m_timeStep;
  std::size_t m_numberOfTimeSteps;
  double m_trueAnomaly;
};

/*
The default and fixed header parameter data for .init files
*/
struct InitHeaderData {
  ~InitHeaderData();

  static std::size_t numberOfBodies();
  static double timeStep(double planetDistance);
  static std::size_t numberOfTimeStep(double planetDistance);
  static double trueAnomaly(double planetDistance);

  static std::unique_ptr<InitHeaderParams> m_fixedHeaderParams;
  static std::map<double, InitHeaderParams> m_defaultHeaderParams;

private:
  static InitHeaderParams const &getDefaultHeaderParams(double planetDistance);
};

/*
The simulation parameters used to calculate body positions and velocities
*/
struct InitSimulationParams {
  InitSimulationParams(std::string const &filename, double pericentre,
                       double planetDistance, std::size_t orientationIndex,
                       std::size_t phi, std::size_t inclination);
  InitSimulationParams(std::string const &filename, double pericentre,
                       double planetDistanceA, double planetDistanceB,
                       std::size_t orientationIndex, std::size_t phi,
                       std::size_t inclination);
  ~InitSimulationParams();

  bool operator!=(InitSimulationParams const &otherParams) const;

  std::string m_filename;
  double m_pericentre;
  std::vector<double> m_planetDistances;
  std::size_t m_orientationIndex;
  std::size_t m_phi;
  std::size_t m_inclination;
};

/*
Other settings used for the simulation
*/
struct OtherSimulationSettings {
  static bool m_hasSinglePlanet;
  static bool m_combinePlanetResults;
  static bool m_useDefaults;
};

#endif /* INITSIMULATIONPARAMS_H */
