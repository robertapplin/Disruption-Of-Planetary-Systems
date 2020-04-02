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
  InitFileGenerator(std::string const &directory);
  ~InitFileGenerator();

  std::vector<SimParameters *> simulationParameters() const;

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
  void generateInitFile(std::string const &initFilename, Body *blackHole,
                        Body *star, Body *planet) const;

  std::string generateFileText(std::string const &initFilename, Body *blackHole,
                               Body *star, Body *planet) const;

  std::string generateInitFilename(std::string const &pericentre,
                                   std::string const &planetDistance,
                                   std::size_t const &orientationIndex) const;

  Body *generateStarData(double pericentre) const;
  Body *generatePlanetData(Body *star, double planetDistance, std::size_t phi,
                           std::size_t inclination) const;

  void addBodyData(std::string &fileText, Body *body) const;

  std::vector<SimParameters *> m_simulationParameters;
  std::unique_ptr<FileManager> m_fileManager;
  std::string m_directory;
};

#endif // !GENERATEINITFILES_H
