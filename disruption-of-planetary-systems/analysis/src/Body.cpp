#include "Body.h"

Body::Body(double mass, std::vector<double> const &position,
           std::vector<double> const &velocity)
    : m_mass(mass), m_position(position), m_velocity(velocity) {}

Body::~Body() {}

double Body::mass() const { return m_mass; }

std::vector<double> Body::position() const { return m_position; }

std::vector<double> Body::velocity() const { return m_velocity; }
