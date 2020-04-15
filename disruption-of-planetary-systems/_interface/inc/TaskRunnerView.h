#ifndef TASKRUNNERVIEW_H
#define TASKRUNNERVIEW_H
#include "ui_TaskRunner.h"

#include <QString>
#include <QWidget>

class TaskRunnerView : public QWidget {
  Q_OBJECT

public:
  TaskRunnerView(QWidget *parent = Q_NULLPTR);
  ~TaskRunnerView();

  void setRunning(bool isRunning);
  void setCancelling(bool isCancelling);

  void setProgressBarText(QString const &text);
  void setProgressBarValue(int value);

signals:
  void actionButtonToggled();

private slots:
  void emitActionButtonToggled();

private:
  void connectUi();

  Ui::TaskRunnerWidget m_ui;
};

#endif /* TASKRUNNERVIEW_H */
