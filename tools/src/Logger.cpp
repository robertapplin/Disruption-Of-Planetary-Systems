// FlashCard Project
// @author Robert Applin
// @start-date 04/07/2019
#include "Logger.h"

#include <QList>
#include <QScrollBar>

void Logger::setLogger(QTextEdit *textEdit, QSplitter *layout) {
  m_logger = textEdit;
  m_layout = layout;
  setStatusOn(true);
}

Logger &Logger::getInstance() {
  static Logger instance;
  return instance;
}

void Logger::setStatusOn(bool statusOn) {
  m_statusOn = statusOn;
  m_logger->setTextColor(Qt::black);
  m_logger->append(m_statusOn ? "Logger status: online"
                              : "Logger status: offline");
}

void Logger::setVisible(bool visible) {
  if (m_statusOn) {
    auto const sizes = visible ? QList<int>{20, 500} : QList<int>{0, 500};
    m_layout->setSizes(sizes);
  }
}

void Logger::addLog(LogType const &logType, std::string const &message) {
  if (m_statusOn) {
    switch (logType) {
    case LogType::Debug:
      m_logger->setTextColor(Qt::black);
      m_logger->append("Debug: " + QString::fromStdString(message));
      break;
    case LogType::Info:
      m_logger->setTextColor(Qt::black);
      m_logger->append("Info: " + QString::fromStdString(message));
      break;
    case LogType::Warning:
      m_logger->setTextColor(Qt::darkYellow);
      m_logger->append("Warning: " + QString::fromStdString(message));
      break;
    case LogType::Error:
      m_logger->setTextColor(Qt::red);
      m_logger->append("Error: " + QString::fromStdString(message));
      setVisible(true);
      break;
    }
    m_logger->verticalScrollBar()->setValue(
        m_logger->verticalScrollBar()->maximum());
  }
}
