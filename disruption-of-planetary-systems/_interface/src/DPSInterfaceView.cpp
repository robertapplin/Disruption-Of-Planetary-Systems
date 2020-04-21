#include "DPSInterfaceView.h"

#include <QRegExp>
#include <QRegExpValidator>

DPSInterfaceView::DPSInterfaceView(QWidget *parent) : QWidget(parent) {
  m_ui.setupUi(this);
  connectUi();

  setPericentresValidator("[0-9,.,,]+");
  setPlanetDistancesAValidator("[0-9,.,,]+");
  setPlanetDistancesBValidator("[0-9,.,,]+");
};

DPSInterfaceView::~DPSInterfaceView() {}

void DPSInterfaceView::connectUi() {
  connect(m_ui.sbNumberOfBodies, SIGNAL(valueChanged(int)), this,
          SLOT(handleNumberOfBodiesChanged(int)));
  connect(m_ui.ckUseDefaultHeaderParameters, SIGNAL(stateChanged(int)), this,
          SLOT(handleUseDefaultHeaderParamsChanged(int)));
}

void DPSInterfaceView::setPericentresValidator(QString const &regex) {
  m_ui.lePericentres->setValidator(createValidator(regex));
}

void DPSInterfaceView::setPlanetDistancesAValidator(QString const &regex) {
  m_ui.lePlanetDistancesA->setValidator(createValidator(regex));
}

void DPSInterfaceView::setPlanetDistancesBValidator(QString const &regex) {
  m_ui.lePlanetDistancesB->setValidator(createValidator(regex));
}

QValidator *DPSInterfaceView::createValidator(QString const &regex) {
  return new QRegExpValidator(QRegExp(regex), this);
}

void DPSInterfaceView::handleNumberOfBodiesChanged(int value) {
  setPlanetDistancesBEnabled(value == 4);
  setPlanetDistanceDefaults(value == 4);
}

void DPSInterfaceView::handleUseDefaultHeaderParamsChanged(int state) {
  setInputHeaderParametersEnabled(state == 0);
}

void DPSInterfaceView::setPlanetDistancesBEnabled(bool enable) {
  m_ui.lePlanetDistancesB->setEnabled(enable);
  m_ui.ckCombinePlanetResults->setEnabled(enable);
}

void DPSInterfaceView::setInputHeaderParametersEnabled(bool enable) {
  m_ui.sbTimeStep->setEnabled(enable);
  m_ui.sbNumberOfTimeSteps->setEnabled(enable);
  m_ui.sbTrueAnomaly->setEnabled(enable);
}

void DPSInterfaceView::setPlanetDistanceDefaults(bool twoPlanets) {
  if (twoPlanets)
    m_ui.lePlanetDistancesA->setText("1,2,3,5,7,10,15,20,28");
  else
    m_ui.lePlanetDistancesA->setText("1,2,3,5,10,20,30,40,50");
}

std::string DPSInterfaceView::directory() const {
  return m_ui.leDirectory->text().toStdString();
}

std::size_t DPSInterfaceView::numberOfBodies() const {
  return static_cast<std::size_t>(m_ui.sbNumberOfBodies->value());
}

bool DPSInterfaceView::combinePlanetResults() const {
  return m_ui.ckCombinePlanetResults->isChecked();
}

bool DPSInterfaceView::useDefaultHeaderParams() const {
  return m_ui.ckUseDefaultHeaderParameters->isChecked();
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

std::string DPSInterfaceView::planetDistancesA() const {
  return m_ui.lePlanetDistancesA->text().toStdString();
}

std::string DPSInterfaceView::planetDistancesB() const {
  return m_ui.lePlanetDistancesB->text().toStdString();
}

std::size_t DPSInterfaceView::numberOfOrientations() const {
  return static_cast<std::size_t>(m_ui.sbNumberOfOrientations->value());
}
