#ifndef BODY_CREATOR_H
#define BODY_CREATOR_H

#include "Body.h"
#include "SimulationConstants.h"

#include <memory>
#include <string>

namespace BodyCreator {

static std::unique_ptr<Body const> blackHole = std::make_unique<Body>(
    SimulationConstants::BH_MASS, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);

std::unique_ptr<Body const> createStar(double pericentre, double trueAnomaly);

std::unique_ptr<Body const> createPlanet(Body const &star,
                                         double planetDistance, std::size_t phi,
                                         std::size_t inclination);

} // namespace BodyCreator

#endif /* BODY_CREATOR_H */
