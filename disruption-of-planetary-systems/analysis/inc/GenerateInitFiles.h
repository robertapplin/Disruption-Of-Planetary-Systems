#ifndef GENERATEINITFILES_H
#define GENERATEINITFILES_H

#include <memory>
#include <string>
#include <vector>

class Body;
class FileManager;
class SimParameters;

class InitFileGenerator {

public:
  InitFileGenerator(std::string const &directory, double timeStep,
                    std::size_t numberOfTimeSteps, double trueAnomaly);
  ~InitFileGenerator();

  void setTimeStep(double timeStep);
  void setNumberOfTimeSteps(std::size_t numberOfTimeSteps);
  void setTrueAnomaly(double trueAnomaly);

  std::vector<SimParameters *> const &simulationParameters() const;

  void generateInitFiles(std::vector<std::string> const &pericentres,
                         std::vector<std::string> const &planetDistances,
                         std::size_t const &numberOfOrientations);

private:
  void resetInitData(std::size_t numberOfInitFiles);
  void updateInitData(std::string const &initFilename, double pericentre,
                      double planetDistance, std::size_t orientationIndex,
                      std::size_t phi, std::size_t inclination);

  void generateInitFile(std::string const &pericentre,
                        std::string const &planetDistance,
                        std::size_t orientationIndex);
  void generateInitFile(std::string const &pericentre,
                        std::string const &planetDistance,
                        std::size_t orientationIndex, std::size_t phi,
                        std::size_t inclination);
  void generateInitFile(std::string const &initFilename,
                        std::string const &pericentre,
                        std::string const &planetDistance, std::size_t phi,
                        std::size_t inclination) const;
  void generateInitFile(std::string const &initFilename, Body const *blackHole,
                        Body const *star, Body const *planet) const;

  std::string generateFileText(std::string const &initFilename,
                               Body const *blackHole, Body const *star,
                               Body const *planet) const;

  std::string generateInitFilename(std::string const &pericentre,
                                   std::string const &planetDistance,
                                   std::size_t const &orientationIndex) const;

  Body const *generateStarData(double pericentre) const;
  Body const *generatePlanetData(Body const *star, double planetDistance,
                                 std::size_t phi,
                                 std::size_t inclination) const;

  void addBodyData(std::string &fileText, Body const *body) const;

  void saveParameters(std::vector<SimParameters *> const &parameters) const;
  std::string
  generateFileText(std::vector<SimParameters *> const &parameters) const;
  std::string generateFileLine(SimParameters const *parameters) const;

  double m_timeStep;
  std::size_t m_numberOfTimeSteps;
  double m_trueAnom;

  std::vector<SimParameters *> m_simulationParameters;
  std::unique_ptr<FileManager> m_fileManager;
  std::string m_directory;
};

#endif // !GENERATEINITFILES_H
