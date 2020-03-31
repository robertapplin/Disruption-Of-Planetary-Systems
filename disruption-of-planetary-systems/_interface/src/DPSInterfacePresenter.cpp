#include "DPSInterfacePresenter.h"
#include "DPSInterfaceModel.h"
#include "DPSInterfaceView.h"

DPSInterfacePresenter::DPSInterfacePresenter(DPSInterfaceView *view,
                                             QWidget *parent)
    : QObject(parent), m_view(view),
      m_model(std::make_unique<DPSInterfaceModel>()) {
  connectPresenter();
};

DPSInterfacePresenter::~DPSInterfacePresenter() {}

void DPSInterfacePresenter::connectPresenter() {
  connect(m_view,
          SIGNAL(runClicked(std::string const &, std::string const &,
                            std::string const &)),
          this,
          SLOT(handleRunClicked(std::string const &, std::string const &,
                                std::string const &)));
}

/*
  Handlers for events in view
*/
void DPSInterfacePresenter::handleRunClicked(
    std::string const &pericentres, std::string const &planetDistances,
    std::string const &numberOfOrientations) {
  m_view->setRunning(true);
  m_model->run(pericentres, planetDistances, numberOfOrientations);
  m_view->setRunning(false);
}
