#ifndef TASK_RUNNER_H
#define TASK_RUNNER_H

#include <string>

#include <QObject>

class TaskRunner : public QObject {
  Q_OBJECT

public:
  TaskRunner(TaskRunner const &) = delete;     // Singleton
  void operator=(TaskRunner const &) = delete; // Singleton

  static TaskRunner &getInstance(); // Singleton

  void startTask();
  void stopTask();

  bool isRunning() const;
  std::string taskDescription() const;
  double startValue() const;
  double endValue() const;
  std::size_t numberOfSteps() const;
  double currentValue() const;

  void setTask(std::string const &task, double start, double end);
  void setNumberOfSteps(std::size_t numberOfSteps);

  void reportProgress();

signals:
  void initializeProgressBar();
  void updateProgressBar();

private:
  TaskRunner() {} // Singleton

  bool m_isRunning = false;
  std::string m_taskDescription = "";
  double m_startValue = 0.0;
  double m_endValue = 100.0;
  std::size_t m_step = 0;
  std::size_t m_numberOfSteps = 100;
};

#endif /* TASK_RUNNER_H */
