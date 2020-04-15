#include "BodyCreator.h"

#include "InitSimulationParams.h"

#include <cmath>

namespace BodyCreator {

using namespace SimulationConstants;

std::unique_ptr<Body const> createStar(double pericentre, double trueAnomaly) {
  // Calculate initial position of star
  auto const x = 2.0 * pericentre * cos(trueAnomaly) / (1 + cos(trueAnomaly));
  auto const y = 2.0 * pericentre * sin(trueAnomaly) / (1 + cos(trueAnomaly));
  auto const xyz = XYZComponents(std::move(x), std::move(y), 0.0);

  // Calculate initial velocity of star
  auto const r = blackHole->relativePositionMagnitude(xyz, BodyTime::Initial);

  auto const parabolicVelocity = sqrt(2.0 * G * blackHole->mass() / r);

  auto const vx = parabolicVelocity * sin(trueAnomaly / 2);
  auto const vy = -parabolicVelocity * cos(trueAnomaly / 2);

  return std::make_unique<Body>(STAR_MASS, x, y, 0.0, vx, vy, 0.0);
}

std::unique_ptr<Body const> createPlanet(Body const &star,
                                         double planetDistance, std::size_t phi,
                                         std::size_t inclination) {
  // Calculate initial position of planet
  auto const starPos = star.position(BodyTime::Initial);

  auto const x = starPos.compX() + planetDistance * cos(inclination) * cos(phi);
  auto const y = starPos.compY() + planetDistance * cos(inclination) * sin(phi);
  auto const z = starPos.compZ() + planetDistance * sin(inclination);

  // Calculate initial velocity of planet
  auto const circularVelocity = sqrt(G * star.mass() / planetDistance);
  auto const starVelocity = star.velocity(BodyTime::Initial);

  auto const vx = starVelocity.compX() + circularVelocity * sin(phi);
  auto const vy = starVelocity.compY() - circularVelocity * cos(phi);

  return std::make_unique<Body>(PLANET_MASS, x, y, z, vx, vy, 0.0);
}

} // namespace BodyCreator
