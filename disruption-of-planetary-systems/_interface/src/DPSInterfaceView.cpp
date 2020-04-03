#include "DPSInterfaceView.h"

#include <QRegExp>
#include <QRegExpValidator>

DPSInterfaceView::DPSInterfaceView(QWidget *parent) : QWidget(parent) {
  m_ui.setupUi(this);
  connectUi();

  setPericentresValidator("[0-9,\.,\,]+");
  setPlanetDistancesValidator("[0-9,\.,\,]+");
};

DPSInterfaceView::~DPSInterfaceView() {}

void DPSInterfaceView::connectUi() {
  connect(m_ui.sbTimeStep, SIGNAL(valueChanged(double)), this,
          SLOT(emitTimeStepChanged(double)));
  connect(m_ui.sbNumberOfTimeSteps, SIGNAL(valueChanged(int)), this,
          SLOT(emitNumberTimeStepsChanged(int)));
  connect(m_ui.sbTrueAnomaly, SIGNAL(valueChanged(double)), this,
          SLOT(emitTrueAnomalyChanged(double)));

  connect(m_ui.pbRun, SIGNAL(clicked()), this, SLOT(emitRunClicked()));
}

void DPSInterfaceView::setPericentresValidator(QString const &regex) {
  m_ui.lePericentres->setValidator(createValidator(regex));
}

void DPSInterfaceView::setPlanetDistancesValidator(QString const &regex) {
  m_ui.lePlanetDistances->setValidator(createValidator(regex));
}

QValidator *DPSInterfaceView::createValidator(QString const &regex) {
  return new QRegExpValidator(QRegExp(regex), this);
}

void DPSInterfaceView::emitTimeStepChanged(double value) {
  emit timeStepChanged(value);
}

void DPSInterfaceView::emitNumberTimeStepsChanged(int value) {
  emit numberTimeStepsChanged(static_cast<std::size_t>(value));
}

void DPSInterfaceView::emitTrueAnomalyChanged(double value) {
  emit trueAnomalyChanged(value);
}

void DPSInterfaceView::emitRunClicked() {
  emit runClicked(pericentres(), planetDistances(), numberOfOrientations());
}

std::string DPSInterfaceView::directory() const {
  return m_ui.leDirectory->text().toStdString();
}

double DPSInterfaceView::timeStep() const { return m_ui.sbTimeStep->value(); }

std::size_t DPSInterfaceView::numberOfTimeSteps() const {
  return static_cast<std::size_t>(m_ui.sbNumberOfTimeSteps->value());
}

double DPSInterfaceView::trueAnomaly() const {
  return m_ui.sbTrueAnomaly->value();
}

std::string DPSInterfaceView::pericentres() const {
  return m_ui.lePericentres->text().toStdString();
}

std::string DPSInterfaceView::planetDistances() const {
  return m_ui.lePlanetDistances->text().toStdString();
}

std::size_t DPSInterfaceView::numberOfOrientations() const {
  return static_cast<std::size_t>(m_ui.sbNumberOfOrientations->value());
}

void DPSInterfaceView::setRunning(bool isRunning) {
  m_ui.pbRun->setEnabled(!isRunning);
  m_ui.pbRun->setText(isRunning ? "Running..." : "Run");
}
