#include "DPSInterfacePresenter.h"
#include "DPSInterfaceModel.h"
#include "DPSInterfaceView.h"

DPSInterfacePresenter::DPSInterfacePresenter(DPSInterfaceView *view,
                                             QWidget *parent)
    : QObject(parent), m_view(view),
      m_model(std::make_unique<DPSInterfaceModel>(
          m_view->directory(), m_view->timeStep(), m_view->numberOfTimeSteps(),
          m_view->trueAnomaly())) {
  connectPresenter();
};

DPSInterfacePresenter::~DPSInterfacePresenter() {}

void DPSInterfacePresenter::connectPresenter() {
  connect(m_view, SIGNAL(timeStepChanged(double)), this,
          SLOT(handleTimeStepChanged(double)));
  connect(m_view, SIGNAL(numberTimeStepsChanged(std::size_t)), this,
          SLOT(handleNumberOfTimeStepsChanged(std::size_t)));
  connect(m_view, SIGNAL(trueAnomalyChanged(double)), this,
          SLOT(handleTrueAnomalyChanged(double)));

  connect(
      m_view,
      SIGNAL(runClicked(std::string const &, std::string const &, std::size_t)),
      this,
      SLOT(handleRunClicked(std::string const &, std::string const &,
                            std::size_t)));
}

/*
  Handlers for events in view
*/
void DPSInterfacePresenter::handleTimeStepChanged(double timeStep) {
  m_model->updateTimeStep(timeStep);
}

void DPSInterfacePresenter::handleNumberOfTimeStepsChanged(
    std::size_t numberOfTimeSteps) {
  m_model->updateNumberOfTimeSteps(numberOfTimeSteps);
}

void DPSInterfacePresenter::handleTrueAnomalyChanged(double trueAnomaly) {
  m_model->updateTrueAnomaly(trueAnomaly);
}

void DPSInterfacePresenter::handleRunClicked(std::string const &pericentres,
                                             std::string const &planetDistances,
                                             std::size_t numberOfOrientations) {
  m_view->setRunning(true);
  m_model->run(pericentres, planetDistances, numberOfOrientations);
  m_view->setRunning(false);
}
