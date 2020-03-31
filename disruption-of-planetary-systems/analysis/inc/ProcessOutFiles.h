#ifndef PROCESSOUTFILES_H
#define PROCESSOUTFILES_H

#include <string>
#include <vector>

class SimParameters;

class OutFileProcessor {
public:
  OutFileProcessor(std::string const &directory);
  ~OutFileProcessor();

  void processOutFiles(
      std::vector<SimParameters *> const &simulationParameters) const;

private:
  void processOutFile(std::string const &filename) const;
  std::string loadOutFile(std::string const &filename) const;
  std::string configureOutData(std::string const &rawData) const;

  void deleteInitFile(std::string const &filename) const;

  std::string m_directory;
};

#endif /* PROCESSOUTFILES_H */
