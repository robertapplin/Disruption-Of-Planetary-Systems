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

  void setRunning(bool isRunning);

signals:
  void runClicked(std::string const &pericentres,
                  std::string const &planetDistances,
                  std::string const &numberOfOrientations);

private slots:
  void emitRunClicked();

private:
  void connectUi();

  void setPericentresValidator(QString const &regex);
  void setPlanetDistancesValidator(QString const &regex);
  void setNumberOfOrientationsValidator(QString const &regex);

  std::string pericentres() const;
  std::string planetDistances() const;
  std::string numberOfOrientations() const;

  QValidator *createValidator(QString const &regex);

  Ui::DPSView m_ui;
};

#endif // !DPSINTERFACEVIEW_H
