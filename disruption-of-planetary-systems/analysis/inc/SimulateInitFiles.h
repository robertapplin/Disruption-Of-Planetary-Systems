#ifndef SIMULATEINITFILES_H
#define SIMULATEINITFILES_H

#include <string>
#include <vector>

class SimParameters;

class InitFileSimulator {

public:
  InitFileSimulator(std::string const &drive, std::string const &subDirectory);
  ~InitFileSimulator();

  void
  simulateInitFiles(std::vector<SimParameters *> const &simulationParameters);

private:
  void simulateFiles(std::vector<SimParameters *> const &initFilenames);

  std::string m_drive;
  std::string m_subDirectory;
};

#endif /* SIMULATEINITFILES_H */
