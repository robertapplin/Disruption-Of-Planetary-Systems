#ifndef SIMULATEINITFILES_H
#define SIMULATEINITFILES_H

#include <memory>
#include <string>
#include <vector>

struct InitSimulationParams;

class FileManager;
class TaskRunner;

class InitFileSimulator {

public:
  InitFileSimulator(std::string const &drive, std::string const &subDirectory);
  ~InitFileSimulator();

  bool simulateInitFiles(
      std::vector<InitSimulationParams> const &simulationParameters);

private:
  void resetSimulator(std::size_t numberOfIntermissions);

  bool simulateInitFiles(
      std::vector<InitSimulationParams> const &simulationParameters,
      std::size_t numberOfIntermissions, std::size_t remainder);
  void simulateInitFiles(
      std::vector<InitSimulationParams>::const_iterator const &beginIter,
      std::size_t numberOfIntermissions, std::size_t remainder,
      std::size_t index) const;
  void simulateInitFiles(
      std::vector<InitSimulationParams>::const_iterator const &startIter,
      std::vector<InitSimulationParams>::const_iterator const &endIter) const;

  std::string getCommand(
      std::vector<InitSimulationParams>::const_iterator const &startIter,
      std::vector<InitSimulationParams>::const_iterator const &endIter) const;

  void deleteInitFiles(
      std::vector<InitSimulationParams> const &simulationParameters) const;
  void deleteFile(std::string const &filename) const;

  std::size_t m_step = 10;

  std::unique_ptr<FileManager> m_fileManager;
  std::string m_drive;
  std::string m_subDirectory;
  std::string m_directory;
  TaskRunner &m_taskRunner;
};

#endif /* SIMULATEINITFILES_H */
