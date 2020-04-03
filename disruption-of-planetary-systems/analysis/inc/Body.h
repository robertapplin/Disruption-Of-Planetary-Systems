#ifndef BODY_H
#define BODY_H

#include <vector>

/// This appears to be an unnecessary data type, but is more explicit
struct XYZComponents {
public:
  XYZComponents(double x, double y, double z) {
    m_xComp = x;
    m_yComp = y;
    m_zComp = z;
  }

  double compX() const { return m_xComp; };
  double compY() const { return m_yComp; };
  double compZ() const { return m_zComp; };

  double magnitude() const {
    return sqrt(pow(m_xComp, 2) + pow(m_yComp, 2) + pow(m_zComp, 2));
  }

  double relativeMag(XYZComponents const &otherXYZ) const {
    return sqrt(pow(m_xComp - otherXYZ.compX(), 2) +
                pow(m_yComp - otherXYZ.compY(), 2) +
                pow(m_zComp - otherXYZ.compZ(), 2));
  };

  XYZComponents crossProduct(XYZComponents const &otherXYZ) const {
    auto const x = m_yComp * otherXYZ.compZ() - m_zComp * otherXYZ.compY();
    auto const y = m_zComp * otherXYZ.compX() - m_xComp * otherXYZ.compZ();
    auto const z = m_xComp * otherXYZ.compY() - m_yComp * otherXYZ.compX();
    return XYZComponents(std::move(x), std::move(y), std::move(z));
  };

  XYZComponents operator-(XYZComponents const &otherXYZ) {
    auto const x = m_xComp - otherXYZ.compX();
    auto const y = m_yComp - otherXYZ.compY();
    auto const z = m_zComp - otherXYZ.compZ();
    return XYZComponents(std::move(x), std::move(y), std::move(z));
  }

private:
  double m_xComp;
  double m_yComp;
  double m_zComp;
};

/// A body consists of a mass, and its positions and velocities at equal
/// intervals in time
enum BodyTime { Initial = 0 } const;

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
  double relativeVelocityMagnitude(Body const &otherBody,
                                   std::size_t index) const;

  std::size_t numberOfTimeSteps() const;

private:
  double m_mass;
  std::vector<XYZComponents> m_positions;
  std::vector<XYZComponents> m_velocities;
};

#endif /* BODY_H */
