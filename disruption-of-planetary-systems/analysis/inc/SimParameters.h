#ifndef SIMPARAMETERS_H
#define SIMPARAMETERS_H

#include <string>

class SimParameters {
public:
  SimParameters(std::string const &filename, double pericentre,
                double planetDistance, std::size_t orientationIndex,
                std::size_t phi, std::size_t inclination);
  ~SimParameters();

  std::string filename() const;
  double pericentre() const;
  double planetDistance() const;
  std::size_t orientationIndex() const;
  std::size_t phi() const;
  std::size_t inclination() const;

private:
  std::string m_filename;

  double m_pericentre;
  double m_planetDistance;
  std::size_t m_orientationIndex;
  std::size_t m_phi;
  std::size_t m_inclination;
};

#endif /* SIMPARAMETERS_H */
