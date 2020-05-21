#include "TaskRunner.h"

TaskRunner &TaskRunner::getInstance() {
  static TaskRunner instance;
  return instance;
}

void TaskRunner::startTask() {
  std::unique_lock<std::mutex> lock(m_mutex);
  m_isRunning = true;
}

void TaskRunner::stopTask() {
  std::unique_lock<std::mutex> lock(m_mutex);
  m_isRunning = false;
}

bool TaskRunner::isRunning() const { return m_isRunning; }

std::string TaskRunner::taskDescription() const { return m_taskDescription; }

double TaskRunner::startValue() const { return m_startValue; }

double TaskRunner::endValue() const { return m_endValue; }

std::size_t TaskRunner::numberOfSteps() const { return m_numberOfSteps; }

double TaskRunner::currentValue() const {
  return m_startValue + (m_endValue - m_startValue) *
                            static_cast<double>(m_step) /
                            static_cast<double>(m_numberOfSteps);
}

void TaskRunner::setTask(std::string const &task, double start, double end) {
  m_taskDescription = task;
  m_startValue = start;
  m_endValue = end;
  emit initializeProgressBar();
}

void TaskRunner::setNumberOfSteps(std::size_t numberOfSteps) {
  m_step = 0;
  m_numberOfSteps = numberOfSteps;
}

void TaskRunner::reportProgress() {
  std::unique_lock<std::mutex> lock(m_mutex);
  if (m_isRunning) {
    ++m_step;
    emit updateProgressBar();
  }
}
