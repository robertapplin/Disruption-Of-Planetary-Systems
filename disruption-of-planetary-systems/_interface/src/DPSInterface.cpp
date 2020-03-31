#include "DPSInterface.h"
#include "DPSInterfacePresenter.h"

#include "Logger.h"

DPSInterface::DPSInterface(QWidget *parent) : QMainWindow(parent) {
  m_ui.setupUi(this);

  m_presenter = std::make_unique<DPSInterfacePresenter>(m_ui.bhcForm, this);

  setupLogger();
}

DPSInterface::~DPSInterface() {}

void DPSInterface::setupLogger() {
  auto &logger = Logger::getInstance();
  logger.setLogger(m_ui.teLogger, m_ui.splitter);
  logger.setVisible(true);
}
