// FlashCard Project
// @author Robert Applin
// @start-date 04/07/2019
#include "FileManager.h"

#include <boost/none.hpp>
#include <fstream>
#include <streambuf>

#include <QFile>
#include <QFileInfo>

FileManager::FileManager() {}

FileManager::FileManager(std::string const &filename) { setFilename(filename); }

FileManager::~FileManager() {}

void FileManager::setFilename(std::string const &filename) {
  m_filename = QString::fromStdString(filename);
}

void FileManager::createNewFile(std::string const &line) const {
  /// You can't write to files which have been compiled into a project using a
  /// qrc file, as these files are restricted to void eexecution problems and
  /// are not writable.
  if (m_filename.isEmpty())
    return;

  QFile file(m_filename);

  if (file.open(QIODevice::WriteOnly)) {
    QTextStream textStream(&file);
    textStream << QString::fromStdString(line);
    file.close();
  } else {
    throw std::runtime_error(
        "Failed to open file " + m_filename.toStdString() +
        " for writing. Please make sure the file is closed.");
  }
}

void FileManager::writeLine(std::string const &line) const {
  /// You can't write to files which have been compiled into a project using a
  /// qrc file, as these files are restricted to void eexecution problems and
  /// are not writable.
  if (m_filename.isEmpty())
    return;

  QFile file(m_filename);

  if (file.open(QIODevice::Append)) {
    QTextStream textStream(&file);
    textStream << QString::fromStdString("\n" + line);
    file.close();
  } else {
    throw std::runtime_error(
        "Failed to open file " + m_filename.toStdString() +
        " for writing. Please make sure the file is closed.");
  }
}

boost::optional<std::string>
FileManager::readLineAtIndex(std::size_t index) const {
  return readLineAtIndex(m_filename, index);
}

boost::optional<std::string>
FileManager::readLineAtIndex(QString const &filename, std::size_t index) const {
  if (m_filename.isEmpty())
    return boost::none;

  QFile file(filename);

  if (file.open(QIODevice::ReadOnly)) {
    QTextStream textStream(&file);

    auto const line = readLineAtIndex(textStream, index);

    file.close();
    return std::move(line);
  }
  throw std::runtime_error("Failed to open file " + m_filename.toStdString() +
                           ".");
}

boost::optional<std::string>
FileManager::readLineAtIndex(QTextStream &textStream, std::size_t index) const {
  std::size_t count(0);

  while (!textStream.atEnd()) {
    auto const line = textStream.readLine().toStdString();

    if (count == index)
      return std::move(line);

    ++count;
  }

  return boost::none;
}

boost::optional<std::string>
FileManager::readPreviousLine(std::string const &line) const {
  if (m_filename.isEmpty())
    return boost::none;

  QFile file(m_filename);

  if (file.open(QIODevice::ReadOnly)) {
    QTextStream textStream(&file);

    auto const previousLine = readPreviousLine(textStream, line);

    file.close();
    return std::move(previousLine);
  }
  throw std::runtime_error("Failed to open file " + m_filename.toStdString() +
                           ".");
}

boost::optional<std::string>
FileManager::readPreviousLine(QTextStream &textStream,
                              std::string const &line) const {
  std::string currentLine;
  std::string previousLine;

  while (!textStream.atEnd()) {
    previousLine = currentLine;
    currentLine = textStream.readLine().toStdString();

    if (currentLine == line && !previousLine.empty())
      return std::move(previousLine);
  }

  return boost::none;
}

boost::optional<std::string>
FileManager::readNextLine(std::string const &line) const {
  if (m_filename.isEmpty())
    return boost::none;

  QFile file(m_filename);

  if (file.open(QIODevice::ReadOnly)) {
    QTextStream textStream(&file);

    auto const nextLine = readNextLine(textStream, line);

    file.close();
    return std::move(nextLine);
  }
  throw std::runtime_error("Failed to open file " + m_filename.toStdString() +
                           ".");
}

boost::optional<std::string>
FileManager::readNextLine(QTextStream &textStream,
                          std::string const &line) const {
  while (!textStream.atEnd())
    if (textStream.readLine().toStdString() == line && !textStream.atEnd())
      return textStream.readLine().toStdString();

  return boost::none;
}
