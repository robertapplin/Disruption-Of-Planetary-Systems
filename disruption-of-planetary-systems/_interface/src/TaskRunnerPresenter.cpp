#include "TaskRunnerPresenter.h"
#include "TaskRunnerView.h"

#include "TaskRunner.h"

#include <boost/algorithm/string.hpp>

namespace {

std::vector<std::string> splitStringByDelimiter(std::string const &str,
                                                std::string const &delimiter) {
  std::vector<std::string> subStrings;
  boost::split(subStrings, str, boost::is_any_of(delimiter));
  return std::move(subStrings);
}

std::string formatDouble(double value, int numberOfDecimalPlaces) {
  auto const subStrings = splitStringByDelimiter(std::to_string(value), ".");
  return subStrings[0] + "." + subStrings[1].substr(0, numberOfDecimalPlaces);
}

} // namespace

TaskRunnerPresenter::TaskRunnerPresenter(TaskRunnerView *view, QWidget *parent)
    : QObject(parent), m_taskRunner(TaskRunner::getInstance()), m_view(view) {
  connectPresenter();
}

TaskRunnerPresenter::~TaskRunnerPresenter() {}

void TaskRunnerPresenter::connectPresenter() {
  connect(m_view, SIGNAL(actionButtonToggled()), this,
          SLOT(handleActionButtonToggled()));
  connect(&m_taskRunner, SIGNAL(initializeProgressBar()), this,
          SLOT(handleInitializeProgressBar()));
  connect(&m_taskRunner, SIGNAL(updateProgressBar()), this,
          SLOT(handleUpdateProgressBar()));
}

void TaskRunnerPresenter::startRunning() {
  m_taskRunner.startTask();
  m_view->setRunning(true);
  emit runClicked();
}

void TaskRunnerPresenter::stopRunning() {
  m_taskRunner.stopTask();
  m_view->setCancelling(true);
  m_view->setProgressBarText("Cancelling...");
}

void TaskRunnerPresenter::handleActionButtonToggled() {
  if (!m_taskRunner.isRunning())
    startRunning();
  else
    stopRunning();
}

void TaskRunnerPresenter::handleUnlockRunning() {
  m_taskRunner.stopTask();
  m_view->setCancelling(false);
  m_view->setProgressBarText("Idle");
  m_view->setProgressBarValue(0);
}

void TaskRunnerPresenter::handleInitializeProgressBar() {
  updateProgressBar(m_taskRunner.startValue());
}

void TaskRunnerPresenter::handleUpdateProgressBar() {
  updateProgressBar(m_taskRunner.currentValue());
}

void TaskRunnerPresenter::updateProgressBar(double value) {
  auto const taskDescription =
      m_taskRunner.taskDescription() + "(" + formatDouble(value, 2) + "%)";
  m_view->setProgressBarText(QString::fromStdString(taskDescription));
  m_view->setProgressBarValue(static_cast<int>(value));
}
