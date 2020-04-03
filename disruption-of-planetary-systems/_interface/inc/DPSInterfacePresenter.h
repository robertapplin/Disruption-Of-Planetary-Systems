#ifndef DPSINTERFACEPRESENTER_H
#define DPSINTERFACEPRESENTER_H

#include <memory>
#include <string>

#include <QObject>
#include <QWidget>

class DPSInterfaceModel;
class DPSInterfaceView;

class DPSInterfacePresenter : public QObject {
  Q_OBJECT

public:
  DPSInterfacePresenter(DPSInterfaceView *view, QWidget *parent = Q_NULLPTR);
  ~DPSInterfacePresenter();

private slots:
  void handleTimeStepChanged(double timeStep);
  void handleNumberOfTimeStepsChanged(std::size_t numberOfTimeSteps);
  void handleTrueAnomalyChanged(double trueAnomaly);

  void handleRunClicked(std::string const &pericentres,
                        std::string const &planetDistances,
                        std::size_t numberOfOrientations);

private:
  void connectPresenter();

  DPSInterfaceView *m_view;
  std::unique_ptr<DPSInterfaceModel> m_model;
};

#endif // !DPSINTERFACEPRESENTER_H
