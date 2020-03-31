#include "PerformanceChecker.h"
#include "Logger.h"

TimeCheck::TimeCheck() : m_start(std::clock()) {}

TimeCheck::~TimeCheck() {}

void TimeCheck::report() const {
  Logger::getInstance().addLog(LogType::Warning,
                               "TimeCheck: " + std::to_string(timeElapsed()));
}

double TimeCheck::timeElapsed() const {
  return (std::clock() - m_start) / (double)CLOCKS_PER_SEC;
}
