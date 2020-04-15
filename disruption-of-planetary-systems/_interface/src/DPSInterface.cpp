#include "DPSInterface.h"
#include "DPSInterfacePresenter.h"
#include "TaskRunnerPresenter.h"

#include "Logger.h"

DPSInterface::DPSInterface(QWidget *parent) : QMainWindow(parent) {
  m_ui.setupUi(this);

  m_presenter = std::make_unique<DPSInterfacePresenter>(m_ui.bhcForm, this);
  m_taskRunnerPresenter =
      std::make_unique<TaskRunnerPresenter>(m_ui.wTaskRunner, this);
  connectPresenters();

  setupLogger();
}

DPSInterface::~DPSInterface() {}

void DPSInterface::connectPresenters() {
  connect(m_taskRunnerPresenter.get(), SIGNAL(runClicked()), m_presenter.get(),
          SLOT(handleRunClicked()));
  connect(m_presenter.get(), SIGNAL(unlockTaskRunner()),
          m_taskRunnerPresenter.get(), SLOT(handleUnlockRunning()));
}

void DPSInterface::setupLogger() {
  auto &logger = Logger::getInstance();
  logger.setLogger(m_ui.teLogger, m_ui.splitter);
  logger.setVisible(true);
}
