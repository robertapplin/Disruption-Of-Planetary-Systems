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

  void unlockRunning();

signals:
  void unlockTaskRunner();

public slots:
  void handleRunClicked();

private:
  void setInitHeaderParams();

  DPSInterfaceView *m_view;
  std::unique_ptr<DPSInterfaceModel> m_model;
};

#endif // !DPSINTERFACEPRESENTER_H
