#ifndef DPSINTERFACEVIEW_H
#define DPSINTERFACEVIEW_H
#include "ui_DPSInterfaceView.h"

#include <string>

#include <QString>
#include <QValidator>
#include <QWidget>

class DPSInterfaceView : public QWidget {
  Q_OBJECT

public:
  DPSInterfaceView(QWidget *parent = Q_NULLPTR);
  ~DPSInterfaceView();

  std::string directory() const;
  std::size_t numberOfBodies() const;
  bool combinePlanetResults() const;
  bool useDefaultHeaderParams() const;
  double timeStep() const;
  std::size_t numberOfTimeSteps() const;
  double trueAnomaly() const;

  std::string pericentres() const;
  std::string planetDistancesA() const;
  std::string planetDistancesB() const;
  std::size_t numberOfOrientations() const;

private slots:
  void handleNumberOfBodiesChanged(int value);
  void handleUseDefaultHeaderParamsChanged(int state);

private:
  void connectUi();

  void setPericentresValidator(QString const &regex);
  void setPlanetDistancesAValidator(QString const &regex);
  void setPlanetDistancesBValidator(QString const &regex);

  void setPlanetDistancesBEnabled(bool enable);
  void setInputHeaderParametersEnabled(bool enable);

  void setPlanetDistanceDefaults(bool twoPlanets);

  QValidator *createValidator(QString const &regex);

  Ui::DPSView m_ui;
};

#endif /* DPSINTERFACEVIEW_H */
