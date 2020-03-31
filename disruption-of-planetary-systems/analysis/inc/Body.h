#ifndef BODY_H
#define BODY_H

#include <vector>

class Body {
public:
  Body(double mass, std::vector<double> const &position,
       std::vector<double> const &velocity);
  ~Body();

  double mass() const;
  std::vector<double> position() const;
  std::vector<double> velocity() const;

private:
  double m_mass;
  std::vector<double> m_position;
  std::vector<double> m_velocity;
};

#endif /* BODY_H */
