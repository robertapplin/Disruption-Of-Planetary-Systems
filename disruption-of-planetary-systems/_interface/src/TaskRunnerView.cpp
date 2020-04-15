#include "TaskRunnerView.h"

TaskRunnerView::TaskRunnerView(QWidget *parent) : QWidget(parent) {
  m_ui.setupUi(this);
  connectUi();
};

TaskRunnerView::~TaskRunnerView() {}

void TaskRunnerView::connectUi() {
  connect(m_ui.pbRun, SIGNAL(clicked()), this, SLOT(emitActionButtonToggled()));
}

void TaskRunnerView::emitActionButtonToggled() { emit actionButtonToggled(); }

void TaskRunnerView::setRunning(bool isRunning) {
  m_ui.pbRun->setText(isRunning ? "Cancel" : "Run");
}

void TaskRunnerView::setCancelling(bool isCancelling) {
  m_ui.pbRun->setEnabled(!isCancelling);
  m_ui.pbRun->setText(isCancelling ? "Canceling..." : "Run");
}

void TaskRunnerView::setProgressBarText(QString const &text) {
  m_ui.bProgressBar->setFormat(text);
}

void TaskRunnerView::setProgressBarValue(int value) {
  m_ui.bProgressBar->setValue(value);
}
