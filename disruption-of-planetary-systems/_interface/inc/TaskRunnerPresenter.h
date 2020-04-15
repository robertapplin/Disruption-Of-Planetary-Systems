#ifndef TASKRUNNERPRESENTER_H
#define TASKRUNNERPRESENTER_H

#include <string>

#include <QObject>
#include <QWidget>

class TaskRunner;
class TaskRunnerView;

class TaskRunnerPresenter : public QObject {
  Q_OBJECT

public:
  TaskRunnerPresenter(TaskRunnerView *view, QWidget *parent = Q_NULLPTR);
  ~TaskRunnerPresenter();

signals:
  void runClicked();

public slots:
  void handleUnlockRunning();

private slots:
  void handleActionButtonToggled();
  void handleInitializeProgressBar();
  void handleUpdateProgressBar();

private:
  void connectPresenter();

  void startRunning();
  void stopRunning();

  void updateProgressBar(double value);

  TaskRunnerView *m_view;
  TaskRunner &m_taskRunner;
};

#endif /* TASKRUNNERPRESENTER_H */
