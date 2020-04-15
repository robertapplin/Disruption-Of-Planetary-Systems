#ifndef XYZCOMPONENTS_H
#define XYZCOMPONENTS_H

struct XYZComponents {
public:
  XYZComponents(double x, double y, double z);
  ~XYZComponents();

  double compX() const;
  double compY() const;
  double compZ() const;

  double magnitude() const;

  double relativeMag(XYZComponents const &otherXYZ) const;

  XYZComponents crossProduct(XYZComponents const &otherXYZ) const;

  XYZComponents operator-(XYZComponents const &otherXYZ);

private:
  double m_xComp;
  double m_yComp;
  double m_zComp;
};

#endif /* XYZCOMPONENTS_H */
