#include "DPSInterfaceView.h"

#include <QRegExp>
#include <QRegExpValidator>

DPSInterfaceView::DPSInterfaceView(QWidget *parent) : QWidget(parent) {
  m_ui.setupUi(this);
  connectUi();

  setPericentresValidator("[0-9,\.,\,]+");
  setPlanetDistancesValidator("[0-9,\.,\,]+");
  setNumberOfOrientationsValidator("[0-9]+");
};

DPSInterfaceView::~DPSInterfaceView() {}

void DPSInterfaceView::connectUi() {
  connect(m_ui.pbRun, SIGNAL(clicked()), this, SLOT(emitRunClicked()));
}

void DPSInterfaceView::setPericentresValidator(QString const &regex) {
  m_ui.lePericentres->setValidator(createValidator(regex));
}

void DPSInterfaceView::setPlanetDistancesValidator(QString const &regex) {
  m_ui.lePlanetDistances->setValidator(createValidator(regex));
}

void DPSInterfaceView::setNumberOfOrientationsValidator(QString const &regex) {
  m_ui.leNumberOrientations->setValidator(createValidator(regex));
}

QValidator *DPSInterfaceView::createValidator(QString const &regex) {
  return new QRegExpValidator(QRegExp(regex), this);
}

void DPSInterfaceView::emitRunClicked() {
  emit runClicked(pericentres(), planetDistances(), numberOfOrientations());
}

std::string DPSInterfaceView::pericentres() const {
  return m_ui.lePericentres->text().toStdString();
}

std::string DPSInterfaceView::planetDistances() const {
  return m_ui.lePlanetDistances->text().toStdString();
}

std::string DPSInterfaceView::numberOfOrientations() const {
  return m_ui.leNumberOrientations->text().toStdString();
}

void DPSInterfaceView::setRunning(bool isRunning) {
  m_ui.pbRun->setEnabled(!isRunning);
  m_ui.pbRun->setText(isRunning ? "Running..." : "Run");
}
