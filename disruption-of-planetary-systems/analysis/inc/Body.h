#ifndef BODY_H
#define BODY_H

#include <vector>

struct XYZComponents;

class Body {
public:
  Body(double mass, XYZComponents const &position,
       XYZComponents const &velocity);
  Body(double mass, std::vector<XYZComponents> const &positions,
       std::vector<XYZComponents> const &velocities);
  ~Body();

  double mass() const;
  XYZComponents position(std::size_t index) const;
  XYZComponents velocity(std::size_t index) const;

  double relativePositionMagnitude(Body const &otherBody,
                                   std::size_t index) const;
  double relativePositionMagnitude(XYZComponents const &otherXYZ,
                                   std::size_t index) const;
  double relativeVelocityMagnitude(Body const &otherBody,
                                   std::size_t index) const;
  double relativeVelocityMagnitude(XYZComponents const &otherXYZ,
                                   std::size_t index) const;

  std::size_t numberOfTimeSteps() const;

private:
  double m_mass;
  std::vector<XYZComponents> m_positions;
  std::vector<XYZComponents> m_velocities;
};

#endif /* BODY_H */
