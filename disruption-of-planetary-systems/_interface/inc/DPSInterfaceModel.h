#ifndef DPSINTERFACEMODEL_H
#define DPSINTERFACEMODEL_H

#include <memory>
#include <string>
#include <vector>

class InitFileGenerator;
class InitFileSimulator;
class OutFileProcessor;

class DPSInterfaceModel {

public:
  DPSInterfaceModel();
  ~DPSInterfaceModel();

  void run(std::string const &pericentreString,
           std::string const &planetDistanceString,
           std::string const &numberOfOrientationString) const;

private:
  void generateInitFiles(std::string const &pericentreString,
                         std::string const &planetDistanceString,
                         std::string const &numberOfOrientationString) const;
  void generateInitFiles(std::vector<std::string> const &pericentres,
                         std::vector<std::string> const &planetDistances,
                         std::size_t numberOfOrientation) const;

  void simulateInitFiles() const;
  void processOutFiles() const;

  std::unique_ptr<InitFileGenerator> m_initFileGenerator;
  std::unique_ptr<InitFileSimulator> m_initFileSimulator;
  std::unique_ptr<OutFileProcessor> m_outFileProcessor;
};

#endif // !DPSINTERFACEMODEL_H
