#ifndef DPSINTERFACE_H
#define DPSINTERFACE_H
#include "ui_DPSInterface.h"

#include <memory>

#include <QWidget>
#include <QtWidgets/QMainWindow>

class DPSInterfacePresenter;
class TaskRunnerPresenter;

class DPSInterface : public QMainWindow {
  Q_OBJECT

public:
  DPSInterface(QWidget *parent = Q_NULLPTR);
  ~DPSInterface();

private:
  void connectPresenters();
  void setupLogger();

  std::unique_ptr<DPSInterfacePresenter> m_presenter;
  std::unique_ptr<TaskRunnerPresenter> m_taskRunnerPresenter;

  Ui::DPSInterfaceClass m_ui;
};

#endif /* DPSINTERFACE_H */
