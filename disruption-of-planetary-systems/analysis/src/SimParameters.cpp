#include "SimParameters.h"

SimParameters::SimParameters(std::string const &filename, double pericentre,
                             double planetDistance,
                             std::size_t orientationIndex, std::size_t phi,
                             std::size_t inclination)
    : m_filename(filename), m_pericentre(pericentre),
      m_planetDistance(planetDistance), m_orientationIndex(orientationIndex),
      m_phi(phi), m_inclination(inclination) {}

SimParameters::~SimParameters() {}

std::string SimParameters::filename() const { return m_filename; }

double SimParameters::pericentre() const { return m_pericentre; }

double SimParameters::planetDistance() const { return m_planetDistance; }

std::size_t SimParameters::orientationIndex() const {
  return m_orientationIndex;
}

std::size_t SimParameters::phi() const { return m_phi; }

std::size_t SimParameters::inclination() const { return m_inclination; }
