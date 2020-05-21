#ifndef BODY_CREATOR_H
#define BODY_CREATOR_H

#include <memory>
#include <string>

class Body;

namespace BodyCreator {

std::unique_ptr<Body const> &blackHole();

std::unique_ptr<Body const> createStar(double pericentre, double trueAnomaly);

std::unique_ptr<Body const> createPlanet(Body const &star,
                                         double planetDistance,
                                         std::size_t orientationIndex,
                                         std::size_t phi,
                                         std::size_t inclination);

} // namespace BodyCreator

#endif /* BODY_CREATOR_H */
