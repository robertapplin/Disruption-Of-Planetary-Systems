#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>
#include <string>
#include <vector>

#include <QSplitter>
#include <QTextEdit>

/*
  Debug: A mode useful for debugging and general information.

  Info: A mode for displaying useful information to the user.

  Warning: A notable 'incident' within the code has occured, however the
  programs execution can continue.

  Error: A crucial operation within the code fails via a 'throw' which is
  caught. The programs execution will have been stopped.
*/
enum LogType { Debug, Info, Warning, Error } const;

class Logger {

public:
  Logger(Logger const &) = delete;         // Singleton
  void operator=(Logger const &) = delete; // Singleton

  void setLogger(QTextEdit *textEdit, QSplitter *layout);

  static Logger &getInstance(); // Singleton

  void setStatusOn(bool statusOn);
  void setVisible(bool visible);

  void addLog(LogType const &logType, std::string const &message);
  void addLogs(LogType const &logType,
               std::vector<std::string> const &messages);

private:
  Logger() {} // Singleton

  QTextEdit *m_logger;
  QSplitter *m_layout;
  bool m_statusOn = false;

  std::mutex m_mutex;
};

#endif /* LOGGER_H */
