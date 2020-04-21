#include "XYZComponents.h"

#include <cmath>
#include <utility>

XYZComponents::XYZComponents(double x, double y, double z)
    : m_xComp(x), m_yComp(y), m_zComp(z) {}

XYZComponents::~XYZComponents() {}

double XYZComponents::compX() const { return m_xComp; };

double XYZComponents::compY() const { return m_yComp; };

double XYZComponents::compZ() const { return m_zComp; };

double XYZComponents::magnitude() const {
  return sqrt(pow(m_xComp, 2) + pow(m_yComp, 2) + pow(m_zComp, 2));
}

double XYZComponents::relativeMag(XYZComponents const &otherXYZ) const {
  return sqrt(pow(m_xComp - otherXYZ.compX(), 2) +
              pow(m_yComp - otherXYZ.compY(), 2) +
              pow(m_zComp - otherXYZ.compZ(), 2));
};

XYZComponents XYZComponents::crossProduct(XYZComponents const &otherXYZ) const {
  auto const x = m_yComp * otherXYZ.compZ() - m_zComp * otherXYZ.compY();
  auto const y = m_zComp * otherXYZ.compX() - m_xComp * otherXYZ.compZ();
  auto const z = m_xComp * otherXYZ.compY() - m_yComp * otherXYZ.compX();
  return XYZComponents(std::move(x), std::move(y), std::move(z));
};

XYZComponents XYZComponents::operator-(XYZComponents const &otherXYZ) {
  auto const x = m_xComp - otherXYZ.compX();
  auto const y = m_yComp - otherXYZ.compY();
  auto const z = m_zComp - otherXYZ.compZ();
  return XYZComponents(std::move(x), std::move(y), std::move(z));
}
