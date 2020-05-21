#ifndef GENERATEINITFILES_H
#define GENERATEINITFILES_H

#include <memory>
#include <string>
#include <vector>

struct InitHeaderParams;
struct InitHeaderDefaults;
struct InitSimulationParams;

class Body;
class FileManager;
class TaskRunner;

class InitFileGenerator {

public:
  InitFileGenerator(std::string const &directory);
  ~InitFileGenerator();

  std::vector<InitSimulationParams> const &simulationParameters() const;

  bool generate(std::vector<std::string> const &pericentres,
                std::vector<std::string> const &planetDistancesA,
                std::vector<std::string> const &planetDistancesB,
                std::size_t numberOfOrientations);

private:
  void resetGenerator(std::size_t numberOfInitFiles);
  void resetInitSimulationParams(std::size_t numberOfInitFiles);
  void addInitSimulationParams(InitSimulationParams const &simParameters);

  bool generateInitFiles(std::vector<std::string> const &pericentres,
                         std::vector<std::string> const &planetDistancesA,
                         std::vector<std::string> const &planetDistancesB,
                         std::size_t numberOfOrientations);
  void generateInitFiles(std::string const &pericentre,
                         std::vector<std::string> const &planetDistancesA,
                         std::vector<std::string> const &planetDistancesB,
                         std::size_t planetDistanceIndex,
                         std::size_t numberOfOrientations);

  void generate3BodyInitFiles(std::string const &pericentre,
                              std::string const &planetDistance,
                              std::size_t numberOfOrientations);
  void generate3BodyInitFile(std::string const &pericentre,
                             std::string const &planetDistance,
                             std::size_t orientationIndex);
  void generate3BodyInitFile(std::string const &pericentre,
                             std::string const &planetDistance,
                             std::size_t orientationIndex, std::size_t phi,
                             std::size_t inclination);
  void generate3BodyInitFile(std::string const &filename, double pericentre,
                             double planetDistance,
                             std::size_t orientationIndex, std::size_t phi,
                             std::size_t inclination);

  void generate4BodyInitFiles(std::string const &pericentre,
                              std::string const &planetDistanceA,
                              std::string const &planetDistanceB,
                              std::size_t numberOfOrientations);
  void generate4BodyInitFile(std::string const &pericentre,
                             std::string const &planetDistanceA,
                             std::string const &planetDistanceB,
                             std::size_t orientationIndex);
  void generate4BodyInitFile(std::string const &pericentre,
                             std::string const &planetDistanceA,
                             std::string const &planetDistanceB,
                             std::size_t orientationIndex, std::size_t phi,
                             std::size_t inclination);
  void generate4BodyInitFile(std::string const &filename, double pericentre,
                             double planetDistanceA, double planetDistanceB,
                             std::size_t orientationIndex, std::size_t phi,
                             std::size_t inclination);

  void createFile(std::string const &filename,
                  std::string const &fileText) const;

  std::string
  generate3BodyInitFilename(std::string const &pericentre,
                            std::string const &planetDistance,
                            std::size_t const &orientationIndex) const;
  std::string
  generate4BodyInitFilename(std::string const &pericentre,
                            std::string const &planetDistanceA,
                            std::string const &planetDistanceB,
                            std::size_t const &orientationIndex) const;

  std::string generateInitHeader(std::string const &filename, double pericentre,
                                 double planetDistance) const;

  void saveSimulationParameters(
      std::vector<InitSimulationParams> const &parameters) const;
  std::string generateSimulationParametersText(
      std::vector<InitSimulationParams> const &parameters) const;
  std::string generateSimulationParametersHeader(
      std::vector<InitSimulationParams> const &parameters) const;
  std::string generateSimulationParametersLine(
      InitSimulationParams const &parameters) const;
  std::string generateSimulationPlanetDistancesSubLine(
      InitSimulationParams const &parameters) const;

  double randomizeTrueAnomaly(double pericentre, double planetDistance) const;

  std::vector<InitSimulationParams> m_simulationParams;

  std::unique_ptr<FileManager> m_fileManager;
  std::string m_directory;
  TaskRunner &m_taskRunner;
};

#endif /* GENERATEINITFILES_H */
