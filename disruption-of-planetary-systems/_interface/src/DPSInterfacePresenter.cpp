#include "DPSInterfacePresenter.h"
#include "DPSInterfaceModel.h"
#include "DPSInterfaceView.h"

#include <QtConcurrent/QtConcurrent>

DPSInterfacePresenter::DPSInterfacePresenter(DPSInterfaceView *view,
                                             QWidget *parent)
    : QObject(parent), m_view(view),
      m_model(std::make_unique<DPSInterfaceModel>(this, m_view->directory())){};

DPSInterfacePresenter::~DPSInterfacePresenter() {}

void DPSInterfacePresenter::setInitHeaderParams() {
  m_model->updateNumberOfBodies(m_view->numberOfBodies());
  m_model->updateUseDefaultHeaderParams(m_view->useDefaultHeaderParams());
  m_model->updateTimeStep(m_view->timeStep());
  m_model->updateNumberOfTimeSteps(m_view->numberOfTimeSteps());
  m_model->updateTrueAnomaly(m_view->trueAnomaly());
}

/*
  Handlers for events in view
*/
void DPSInterfacePresenter::handleRunClicked() {
  setInitHeaderParams();
  QtConcurrent::run(m_model.get(), &DPSInterfaceModel::run,
                    m_view->pericentres(), m_view->planetDistancesA(),
                    m_view->planetDistancesB(), m_view->numberOfOrientations());
}

/*
  Status change in the model
*/
void DPSInterfacePresenter::unlockRunning() { emit unlockTaskRunner(); }
