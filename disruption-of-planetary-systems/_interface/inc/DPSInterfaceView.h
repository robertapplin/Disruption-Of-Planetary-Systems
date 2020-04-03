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
  double timeStep() const;
  std::size_t numberOfTimeSteps() const;
  double trueAnomaly() const;

  void setRunning(bool isRunning);

signals:
  void timeStepChanged(double value);
  void numberTimeStepsChanged(std::size_t value);
  void trueAnomalyChanged(double value);
  void runClicked(std::string const &pericentres,
                  std::string const &planetDistances,
                  std::size_t numberOfOrientations);

private slots:
  void emitTimeStepChanged(double value);
  void emitNumberTimeStepsChanged(int value);
  void emitTrueAnomalyChanged(double value);
  void emitRunClicked();

private:
  void connectUi();

  void setPericentresValidator(QString const &regex);
  void setPlanetDistancesValidator(QString const &regex);

  std::string pericentres() const;
  std::string planetDistances() const;
  std::size_t numberOfOrientations() const;

  QValidator *createValidator(QString const &regex);

  Ui::DPSView m_ui;
};

#endif // !DPSINTERFACEVIEW_H
