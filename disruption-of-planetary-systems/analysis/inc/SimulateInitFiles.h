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
  std::string
  getCommand(std::vector<SimParameters *> const &simulationParameters) const;

  void deleteInitFiles(
      std::vector<SimParameters *> const &simulationParameters) const;
  void deleteFile(std::string const &filename) const;

  std::string m_drive;
  std::string m_subDirectory;
  std::string m_directory;
};

#endif /* SIMULATEINITFILES_H */
