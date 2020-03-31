// FlashCard Project
// @author Robert Applin
// @start-date 04/07/2019
#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <boost/optional.hpp>

#include <QString>
#include <QTextStream>

class FileManager {

public:
  FileManager(std::string const &filename);
  ~FileManager();

  void setFilename(std::string const &filename);

  void createNewFile(std::string const &line) const;
  void writeLine(std::string const &line) const;

  boost::optional<std::string> readAll() const;
  boost::optional<std::string> readLineAtIndex(std::size_t index) const;
  boost::optional<std::string> readLineAtIndex(QString const &filename,
                                               std::size_t index) const;
  boost::optional<std::string> readPreviousLine(std::string const &line) const;

  boost::optional<std::string> readNextLine(std::string const &line) const;

private:
  boost::optional<std::string> readLineAtIndex(QTextStream &textStream,
                                               std::size_t index) const;
  boost::optional<std::string> readPreviousLine(QTextStream &textStream,
                                                std::string const &line) const;
  boost::optional<std::string> readNextLine(QTextStream &textStream,
                                            std::string const &line) const;

  QString m_filename;
};

#endif /* FILEMANAGER_H */
