#include "Body.h"

Body::Body(double mass, XYZComponents const &position,
           XYZComponents const &velocity)
    : m_mass(mass) {
  m_positions.emplace_back(position);
  m_velocities.emplace_back(velocity);
}

Body::Body(double mass, std::vector<XYZComponents> const &positions,
           std::vector<XYZComponents> const &velocities)
    : m_mass(mass), m_positions(positions), m_velocities(velocities) {}

Body::~Body() {}

double Body::mass() const { return m_mass; }

XYZComponents Body::position(std::size_t index) const {
  if (index < m_positions.size())
    return m_positions[index];
  throw std::runtime_error("Position index is out of range.");
}

XYZComponents Body::velocity(std::size_t index) const {
  if (index < m_velocities.size())
    return m_velocities[index];
  throw std::runtime_error("Velocity index is out of range.");
}

double Body::relativePositionMagnitude(Body const &otherBody,
                                       std::size_t index) const {
  return position(index).relativeMag(otherBody.position(index));
}

double Body::relativeVelocityMagnitude(Body const &otherBody,
                                       std::size_t index) const {
  return velocity(index).relativeMag(otherBody.velocity(index));
}

std::size_t Body::numberOfTimeSteps() const { return m_positions.size(); }
